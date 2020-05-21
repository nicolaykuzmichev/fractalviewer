#pragma once

#include <string.h>
#include <fstream>

namespace FractalViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public value class PointD						// Точка в "мировой" системе координат
	{
	public:
		Double X, Y;
		PointD(Double x, Double y) : X(x), Y(y) {}
	};

	public enum FractalType							// Тип фрактала
	{
		CONSTRUCT,									// Конструктивный фрактал
		IFSD,										// Детерминированная СИФ
		IFSR,										// Рандомизированная СИФ
		MANDELBROT,									// Фрактал Мандельброта
		JULIA										// Фрактал Жулиа
		//LORENZ_ZX,								// Странный аттрактор Лоренца(Z-X плоскость)
		//LORENZ_XY									// Странный аттрактор Лоренца(X-Y плоскость)
	};

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		System::Drawing::Graphics ^G;				// Graphics для конструктивных фракталов
		System::Drawing::Graphics ^GB;				// Graphics для остальных фракталов
		System::Drawing::Pen ^P;					// Кисть
		System::Drawing::SolidBrush ^B;				// Заливка заднего плана
		System::Drawing::Bitmap ^Bmp;				// Область отрисовки

		Generic::List<PointD> ^OriginBase;			// "Слепок" основы
		Generic::List<PointD> ^OriginPattern;		// "Слепок" шаблона
		Generic::List<PointD> ^Base;				// Рабочая основа
		Generic::List<PointD> ^Pattern;				// Рабочий шаблон
		Generic::List<PointD> ^Img;					// Образ основы

		FractalType type = CONSTRUCT;				// Текущий тип фрактала

		array<Double, 2> ^C;						// Матрица коэффициентов преобразований
		array<Byte, 2> ^E0;							// Начальное множество E0
		int NumIters;								// Число итераций
		int NumAffine;								// Число преобразований
		int SizePanel;								// Размер квадрата для отрисовки

		Double Xmin, Xmax, Ymin, Ymax;				// Переменные области отрисовки фрактала

		Form ^AboutBox;								// Форма раздела "О программе"
		
		System::IO::StreamReader^ DataIn;			// Поток открытого файла фрактала
		String^ DataStr;							// Отдельная строка из потока
		array<String^>^ result;						// Результат парсинга потока
	
	public:
	private: System::Windows::Forms::ColorDialog^  colorDialog1;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^  selectBackgroundColorToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^  openFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  examplesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  minkowskiFractalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sierpinskiFractalIFSDToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sierpinskiFractalIFSRToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  fernToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  leafToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  crystalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mandelbrotFractalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  juliaFractalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  lorenzSystemToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  lorenzSystemXYToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  continuousModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  inStepModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem3;
	private: System::Windows::Forms::ToolStripMenuItem^  drawToolStripMenuItem;
	public:

		MainForm(void)
		{

			InitializeComponent();

			//Xmin = -2; Ymin = -2; Xmax = 2; Ymax = 2;
			Xmin = -0.1; Ymin = -0.7; Xmax = 1.1; Ymax = 0.7;

			G = panel1->CreateGraphics();						
			P = gcnew Pen(Color::Orange);							
			B = gcnew SolidBrush(Color::Black);		

			Bmp = gcnew Bitmap(panel1->Width, panel1->Height);  
			GB = Graphics::FromImage(Bmp);

			// Размер области для отрисовки Bitmap - наименьший из размеров panel1 (Width, Height) 
			SizePanel = ((panel1->Width <= panel1->Height) ? panel1->Width : panel1->Height) / 1.001;

			OriginBase = gcnew Generic::List<PointD>();
			OriginPattern = gcnew Generic::List<PointD>();
			Base = gcnew Generic::List<PointD>();
			Pattern = gcnew Generic::List<PointD>();
			Img = gcnew Generic::List<PointD>();

			// Загрузка фрактала "по умолчанию"
			this->DefaultBaseAndPattern();	
			this->ClearPanel();
		}

	protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  clearToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  settingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::Panel^  panel1;


	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->drawToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->clearToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->continuousModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->inStepModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->selectBackgroundColorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->examplesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->minkowskiFractalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sierpinskiFractalIFSDToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sierpinskiFractalIFSRToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fernToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->leafToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->crystalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mandelbrotFractalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->juliaFractalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lorenzSystemToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lorenzSystemXYToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->fileToolStripMenuItem,
					this->editToolStripMenuItem, this->examplesToolStripMenuItem, this->helpToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(682, 28);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->openFileToolStripMenuItem,
					this->saveToolStripMenuItem, this->toolStripMenuItem1, this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(44, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openFileToolStripMenuItem
			// 
			this->openFileToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"openFileToolStripMenuItem.Image")));
			this->openFileToolStripMenuItem->Name = L"openFileToolStripMenuItem";
			this->openFileToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->openFileToolStripMenuItem->Size = System::Drawing::Size(208, 26);
			this->openFileToolStripMenuItem->Text = L"Open file";
			this->openFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openFileToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"saveToolStripMenuItem.Image")));
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->ShortcutKeyDisplayString = L"Ctrl+S";
			this->saveToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->saveToolStripMenuItem->Size = System::Drawing::Size(208, 26);
			this->saveToolStripMenuItem->Text = L"Save to file";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(205, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"exitToolStripMenuItem.Image")));
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->ShortcutKeyDisplayString = L"Ctrl+E";
			this->exitToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::E));
			this->exitToolStripMenuItem->Size = System::Drawing::Size(208, 26);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitToolStripMenuItem_Click);
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {
				this->drawToolStripMenuItem,
					this->clearToolStripMenuItem, this->toolStripMenuItem2, this->continuousModeToolStripMenuItem, this->inStepModeToolStripMenuItem,
					this->toolStripMenuItem3, this->settingsToolStripMenuItem, this->selectBackgroundColorToolStripMenuItem
			});
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(53, 24);
			this->editToolStripMenuItem->Text = L"View";
			// 
			// drawToolStripMenuItem
			// 
			this->drawToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"drawToolStripMenuItem.Image")));
			this->drawToolStripMenuItem->Name = L"drawToolStripMenuItem";
			this->drawToolStripMenuItem->ShortcutKeyDisplayString = L"Alt+D";
			this->drawToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::D));
			this->drawToolStripMenuItem->Size = System::Drawing::Size(245, 26);
			this->drawToolStripMenuItem->Text = L"Draw";
			this->drawToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::drawToolStripMenuItem_Click);
			// 
			// clearToolStripMenuItem
			// 
			this->clearToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"clearToolStripMenuItem.Image")));
			this->clearToolStripMenuItem->Name = L"clearToolStripMenuItem";
			this->clearToolStripMenuItem->ShortcutKeyDisplayString = L"Ctrl+C";
			this->clearToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::C));
			this->clearToolStripMenuItem->Size = System::Drawing::Size(245, 26);
			this->clearToolStripMenuItem->Text = L"Clear";
			this->clearToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::clearToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(242, 6);
			// 
			// continuousModeToolStripMenuItem
			// 
			this->continuousModeToolStripMenuItem->Checked = true;
			this->continuousModeToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->continuousModeToolStripMenuItem->Name = L"continuousModeToolStripMenuItem";
			this->continuousModeToolStripMenuItem->Size = System::Drawing::Size(245, 26);
			this->continuousModeToolStripMenuItem->Text = L"Continuous mode";
			this->continuousModeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::continuousModeToolStripMenuItem_Click);
			// 
			// inStepModeToolStripMenuItem
			// 
			this->inStepModeToolStripMenuItem->Name = L"inStepModeToolStripMenuItem";
			this->inStepModeToolStripMenuItem->Size = System::Drawing::Size(245, 26);
			this->inStepModeToolStripMenuItem->Text = L"In step mode";
			this->inStepModeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::inStepModeToolStripMenuItem_Click);
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(242, 6);
			// 
			// settingsToolStripMenuItem
			// 
			this->settingsToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"settingsToolStripMenuItem.Image")));
			this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
			this->settingsToolStripMenuItem->Size = System::Drawing::Size(245, 26);
			this->settingsToolStripMenuItem->Text = L"Select pen color";
			this->settingsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::settingsToolStripMenuItem_Click);
			// 
			// selectBackgroundColorToolStripMenuItem
			// 
			this->selectBackgroundColorToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"selectBackgroundColorToolStripMenuItem.Image")));
			this->selectBackgroundColorToolStripMenuItem->Name = L"selectBackgroundColorToolStripMenuItem";
			this->selectBackgroundColorToolStripMenuItem->Size = System::Drawing::Size(245, 26);
			this->selectBackgroundColorToolStripMenuItem->Text = L"Select background color";
			this->selectBackgroundColorToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::selectBackgroundColorToolStripMenuItem_Click);
			// 
			// examplesToolStripMenuItem
			// 
			this->examplesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {
				this->minkowskiFractalToolStripMenuItem,
					this->sierpinskiFractalIFSDToolStripMenuItem, this->sierpinskiFractalIFSRToolStripMenuItem, this->fernToolStripMenuItem, this->leafToolStripMenuItem,
					this->crystalToolStripMenuItem, this->mandelbrotFractalToolStripMenuItem, this->juliaFractalToolStripMenuItem, this->lorenzSystemToolStripMenuItem,
					this->lorenzSystemXYToolStripMenuItem
			});
			this->examplesToolStripMenuItem->Name = L"examplesToolStripMenuItem";
			this->examplesToolStripMenuItem->Size = System::Drawing::Size(84, 24);
			this->examplesToolStripMenuItem->Text = L"Examples";
			// 
			// minkowskiFractalToolStripMenuItem
			// 
			this->minkowskiFractalToolStripMenuItem->Name = L"minkowskiFractalToolStripMenuItem";
			this->minkowskiFractalToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->minkowskiFractalToolStripMenuItem->Text = L"Minkowski fractal";
			this->minkowskiFractalToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::minkowskiFractalToolStripMenuItem_Click);
			// 
			// sierpinskiFractalIFSDToolStripMenuItem
			// 
			this->sierpinskiFractalIFSDToolStripMenuItem->Name = L"sierpinskiFractalIFSDToolStripMenuItem";
			this->sierpinskiFractalIFSDToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->sierpinskiFractalIFSDToolStripMenuItem->Text = L"Sierpinski fractal - IFSD";
			this->sierpinskiFractalIFSDToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::sierpinskiFractalIFSDToolStripMenuItem_Click);
			// 
			// sierpinskiFractalIFSRToolStripMenuItem
			// 
			this->sierpinskiFractalIFSRToolStripMenuItem->Name = L"sierpinskiFractalIFSRToolStripMenuItem";
			this->sierpinskiFractalIFSRToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->sierpinskiFractalIFSRToolStripMenuItem->Text = L"Sierpinski fractal - IFSR";
			this->sierpinskiFractalIFSRToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::sierpinskiFractalIFSRToolStripMenuItem_Click);
			// 
			// fernToolStripMenuItem
			// 
			this->fernToolStripMenuItem->Name = L"fernToolStripMenuItem";
			this->fernToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->fernToolStripMenuItem->Text = L"Fern";
			this->fernToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::fernToolStripMenuItem_Click);
			// 
			// leafToolStripMenuItem
			// 
			this->leafToolStripMenuItem->Name = L"leafToolStripMenuItem";
			this->leafToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->leafToolStripMenuItem->Text = L"Leaf";
			this->leafToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::leafToolStripMenuItem_Click);
			// 
			// crystalToolStripMenuItem
			// 
			this->crystalToolStripMenuItem->Name = L"crystalToolStripMenuItem";
			this->crystalToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->crystalToolStripMenuItem->Text = L"Crystal";
			this->crystalToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::crystalToolStripMenuItem_Click);
			// 
			// mandelbrotFractalToolStripMenuItem
			// 
			this->mandelbrotFractalToolStripMenuItem->Name = L"mandelbrotFractalToolStripMenuItem";
			this->mandelbrotFractalToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->mandelbrotFractalToolStripMenuItem->Text = L"Mandelbrot fractal";
			this->mandelbrotFractalToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::mandelbrotFractalToolStripMenuItem_Click);
			// 
			// juliaFractalToolStripMenuItem
			// 
			this->juliaFractalToolStripMenuItem->Name = L"juliaFractalToolStripMenuItem";
			this->juliaFractalToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->juliaFractalToolStripMenuItem->Text = L"Julia fractal";
			this->juliaFractalToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::juliaFractalToolStripMenuItem_Click);
			// 
			// lorenzSystemToolStripMenuItem
			// 
			this->lorenzSystemToolStripMenuItem->Name = L"lorenzSystemToolStripMenuItem";
			this->lorenzSystemToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->lorenzSystemToolStripMenuItem->Text = L"Lorenz System (Z-X)";
			this->lorenzSystemToolStripMenuItem->Visible = false;
			this->lorenzSystemToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::lorenzSystemZXToolStripMenuItem_Click);
			// 
			// lorenzSystemXYToolStripMenuItem
			// 
			this->lorenzSystemXYToolStripMenuItem->Name = L"lorenzSystemXYToolStripMenuItem";
			this->lorenzSystemXYToolStripMenuItem->Size = System::Drawing::Size(237, 26);
			this->lorenzSystemXYToolStripMenuItem->Text = L"Lorenz System (X-Y)";
			this->lorenzSystemXYToolStripMenuItem->Visible = false;
			this->lorenzSystemXYToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::lorenzSystemXYToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->aboutToolStripMenuItem });
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(53, 24);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"aboutToolStripMenuItem.Image")));
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F1;
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(178, 26);
			this->aboutToolStripMenuItem->Text = L"About Box";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::aboutToolStripMenuItem_Click);
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(0, 29);
			this->panel1->Margin = System::Windows::Forms::Padding(4);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(682, 625);
			this->panel1->TabIndex = 1;
			// 
			// colorDialog1
			// 
			this->colorDialog1->AnyColor = true;
			this->colorDialog1->Color = System::Drawing::Color::DodgerBlue;
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->Filter = L"Jpeg File(*.jpg;)|*.JPG;";
			this->saveFileDialog1->RestoreDirectory = true;
			this->saveFileDialog1->Title = L"Saving Fractal Picture";
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->DefaultExt = L"txt";
			this->openFileDialog1->Title = L"Open file with fractal set";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(682, 653);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip1);
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4);
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FractalViewer";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::MainForm_Paint);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:

	// Перевод из "мировой" системы координат в экранную
	private: System::Void ToScreen(PointD W, Point &S)
	{
		S.X = (W.X - Xmin) / (Xmax - Xmin)*panel1->Width;
		S.Y = (Ymax - W.Y) / (Ymax - Ymin)*panel1->Height;
	}

	// Перевод из "экранной" системы координат в мировую
	private: System::Void ToWorld(Point S, PointD &W)
	{
		W.X = Xmin + S.X*(Xmax - Xmin) / panel1->Width;
		W.Y = Ymax - S.Y*(Ymax - Ymin) / panel1->Height;
	}

	// Отрисовка линии по двум точкам
	private: System::Void DrawLineW(PointD W1, PointD W2)
	{
		Point S1, S2;
		this->ToScreen(W1, S1);
		this->ToScreen(W2, S2);
		G->DrawLine(P, S1, S2);
		GB->DrawLine(P, S1, S2);
	}

	// Выход из приложения
	private: System::Void exitToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->Close();
	}

	// Пункт меню очистки панели для отрисовки фрактала
	private: System::Void clearToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->ClearPanel();
		
		NumIters = 0;

		this->ClearBaseAndPattern();
		this->LoadOriginBaseAndPattern();
	}

	// Очистка панели для отрисовки фрактала
	private: System::Void ClearPanel(Void)
	{
		G->FillRectangle(B, 0, 0, panel1->Width, panel1->Height);
		GB->FillRectangle(B, 0, 0, panel1->Width, panel1->Height);
	}
	
	// Очистка базы и паттерна от вычисленного контура конструктивного фрактала
	private: System::Void ClearBaseAndPattern(Void)
	{
		Base->Clear();
		Pattern->Clear();
		Img->Clear();
	}

	// Загрузка в рабочую базу и паттерн "слепка" конструктивного фрактала
	private: System::Void LoadOriginBaseAndPattern(Void)
	{
		Base->AddRange(OriginBase);
		Pattern->AddRange(OriginPattern);
	}

	// Загрузка базы и паттерна конструктивного фрактала ("по умолчанию")
	private: System::Void DefaultBaseAndPattern(Void)
	{
		OriginBase->Add(PointD(0, 0));
		OriginBase->Add(PointD(1, 0));

		OriginPattern->Add(PointD(0, 0));
		OriginPattern->Add(PointD(0.25, 0));
		OriginPattern->Add(PointD(0.25, -0.25));
		OriginPattern->Add(PointD(0.5, -0.25));
		OriginPattern->Add(PointD(0.5, 0.25));
		OriginPattern->Add(PointD(0.75, 0.25));
		OriginPattern->Add(PointD(0.75, 0));
		OriginPattern->Add(PointD(1, 0));

		this->LoadOriginBaseAndPattern();
	}

	// Процедура вычисления и отрисовки конструктивного фрактала
	private: System::Void DrawConstruct(Void)
	{
		for (unsigned long long i = 0; i < Base->Count - 1; i++)
		{
			PointD LSegment = Base[i],
				   RSegment = Base[i + 1];
			
			// вычисление координат внутренних  точек сегментов 
			for (int j = 0; j < Pattern->Count; j++)
			{
				PointD point;
				point.X = LSegment.X
								+ (RSegment.X - LSegment.X) * Pattern[j].X
								- (RSegment.Y - LSegment.Y) * Pattern[j].Y;

				point.Y = LSegment.Y
								+ (RSegment.Y - LSegment.Y) * Pattern[j].X
								+ (RSegment.X - LSegment.X) * Pattern[j].Y;
				
				Img->Add(point);
			}
		}

		Generic::List<PointD> ^intmd_pointList = Base;
		Base = Img;
		intmd_pointList->Clear();
		Img = intmd_pointList;

		// отрисовка изображения
		for (unsigned long long i = 0, j = (Base->Count - 1); i < (Base->Count)/2, j >=(Base->Count)/2; i++, j--)
		{
			DrawLineW(Base[i], Base[i + 1]);
			DrawLineW(Base[j], Base[j - 1]);
		}
	}

	// Процедура вычисления и отрисовки фрактала детерминированной СИФ
	private: System::Void DrawIFSD(
		int NumIters, int NumAffine, array<Double, 2> ^C, array<Byte, 2> ^E0, int SizePanel)
	{
		int iter = (inStepModeToolStripMenuItem->Checked) ? 1 : 20;

		Xmin = 0; Ymin = 0; Xmax = 1; Ymax = 1;

		while(iter--)
		{
			array<Byte, 2> ^T = E0;			// матрица аттрактора

			for (int k = 0; k < NumIters; k++)
			{
				array<Byte, 2> ^SM = gcnew array<Byte, 2>(SizePanel, SizePanel);		// матрица атттарктора на текущей итерации

				// выполнение для каждого ненулевого элемента
				for (int i = 0; i < SizePanel; i++)
					for (int j = 0; j < SizePanel; j++)
						if (T[i, j] == 1)
							for (int Affine = 0; Affine < NumAffine; Affine++)
							{
								// перевод из "экранной" системы координат в "мировую"
								PointD W(0, 0);
								ToWorld(Point(i, j), W);

								// применение афинных преобразаований к точке
								double X = C[Affine, 0] * W.X + C[Affine, 1] * W.Y + C[Affine, 4];
								double Y = C[Affine, 2] * W.X + C[Affine, 3] * W.Y + C[Affine, 5];

								// перевод из "мировой" системы координат в "экранную"
								Point S(0, 0);
								ToScreen(PointD(X, Y), S);

								// сохранение измененных координат точки, если она входит в область отрисовки
								if ((S.X >= 0) && (S.X < SizePanel) && (S.Y >= 0) && (S.Y < SizePanel))
									SM[S.X, S.Y] = 1;
							}
				T = SM;
				delete SM;
			}

			// отрисовка изображения
			for (int i = 0; i < SizePanel; i++)
			{
				for (int j = 0; j < SizePanel; j++)
					if (T[i, j] == 1)
						Bmp->SetPixel(i, j, P->Color);
					else
						Bmp->SetPixel(i, j, B->Color);
				if ((inStepModeToolStripMenuItem->Checked) && (i % 3 == 0))
					G->DrawImage(Bmp, 0, 0);
			}

			if (continuousModeToolStripMenuItem->Checked)
			{
				G->DrawImage(Bmp, 0, 0);
				NumIters++;
			}
		}

		Xmin = -0.1; Ymin = -0.7; Xmax = 1.1; Ymax = 0.7;
	}

	// Процедура вычисления и отрисовки фрактала рандомизированной СИФ(только ковер Серпинского - альфа версия алгоритма)
	private: System::Void DrawIFSR(Void)
	{
		Xmin = -0.2; Ymin = -1; Xmax = 2; Ymax = 1;

		int NumIters = 100000;		// число итераций (больше - четче изображение)
		int ItersForPrint = 100;	// через какое число итераций выводить получившееся изображение
		int NumAffine = 3;			// число преобразований и аттракторов
		double X0 = 0, Y0 = 0;		// координаты начальной точки
		double X = 0, Y = 0;		// координаты текущей точки

		System::Random^ Rand = gcnew System::Random();					// экземпляр класса Random для получения равномерной последовательности случайных чисел

		array<Double, 2> ^C = gcnew array<Double, 2>(NumAffine, 6);		// матрица коэффициентов преобразований

		// Ковер Серпинского
		C[0, 0] = 0.5;		C[0, 1] = 0;	C[0, 2] = 0;	C[0, 3] = 0.5;		C[0, 4] = 0;		C[0, 5] = 0;
		C[1, 0] = 0.5;		C[1, 1] = 0;	C[1, 2] = 0;	C[1, 3] = 0.5;		C[1, 4] = 0.5;		C[1, 5] = 0;
		C[2, 0] = 0.5;		C[2, 1] = 0;	C[2, 2] = 0;	C[2, 3] = 0.5;		C[2, 4] = 0.25;		C[2, 5] = 0.43301;

		/*// Лист
		C[0, 0] = 0.4;		C[0, 1] = -0.373;	C[0, 2] = 0.06;		C[0, 3] = 0.6;		C[0, 4] = 0.353;	C[0, 5] = 0;
		C[1, 0] = -0.8;		C[1, 1] = 0.186;	C[1, 2] = 0.137;	C[1, 3] = 0.8;		C[1, 4] = 1.1;		C[1, 5] = 0.1;*/

		// цикл "доводки" произвольной точки
		for (int i = 0; i < 100; i++)
		{
			int Affine = Rand->Next(0, NumAffine);

			X = C[Affine, 0] * X0 + C[Affine, 1] * Y0 + C[Affine, 4];
			Y = C[Affine, 2] * X0 + C[Affine, 3] * Y0 + C[Affine, 5];

			X0 = X;
			Y0 = Y;
		}

		// основной цикл вычисления и отрисовки фрактала IFSR
		for (int i = 0; i < NumIters; i++)
		{
			int Affine = Rand->Next(0, NumAffine);
			
			X = C[Affine, 0] * X0 + C[Affine, 1] * Y0 + C[Affine, 4];
			Y = C[Affine, 2] * X0 + C[Affine, 3] * Y0 + C[Affine, 5];

			X0 = X;
			Y0 = Y;

			Point S;
			ToScreen(PointD(X, Y), S);

			// отрисовка изображения каждые ItersForPrint раз
			if ((S.X >= 0) && (S.X < SizePanel) && (S.Y >= 0) && (S.Y < SizePanel))
				Bmp->SetPixel(S.X, S.Y, P->Color);
				if (i % ItersForPrint == 0)
					G->DrawImage(Bmp, 100, 100);
		}

		Xmin = -0.1; Ymin = -0.7; Xmax = 1.1; Ymax = 0.7;
	}

	// Процедура вычисления и отрисовки фрактала Жулиа(0) и Мандельброта(1)
	private: System::Void DrawJulia(Void)
	{
		Xmin = -2; Ymin = -2; Xmax = 2; Ymax = 2;

		int iter = (inStepModeToolStripMenuItem->Checked)? 1 : 100;

		while (iter--)
		{
			for (int i = 0; i < panel1->Width; i++)
			{
				for (int j = 0; j < panel1->Height; j++)
				{
					PointD W, C(-0.22, -0.74), WT;
					ToWorld(Point(i, j), W);

					for (int k = 0; k < NumIters; k++)
					{
						// преобразование точек
						WT.X = W.X * W.X - W.Y * W.Y + C.X;
						WT.Y = 2 * W.X * W.Y + C.Y;

						// выделение точек, не принадлежащих множеству
						if (WT.X * WT.X + WT.Y * WT.Y > 4)
						{
							Bmp->SetPixel(i, j, B->Color);
							break;
						}

						W.X = WT.X;
						W.Y = WT.Y;
					}

					// выделение точек, принадлежащих множеству
					if (WT.X * WT.X + WT.Y * WT.Y < 4)
						Bmp->SetPixel(i, j, P->Color);
				}

				//отрисовка изображения
				if ((inStepModeToolStripMenuItem->Checked) && (i % 3 == 0))
					G->DrawImage(Bmp, 0, 0);
			}

			if (continuousModeToolStripMenuItem->Checked)
			{
				G->DrawImage(Bmp, 0, 0);
				NumIters++;
			}
		}

		Xmin = -0.1; Ymin = -0.7; Xmax = 1.1; Ymax = 0.7;
	}
	
	private: System::Void DrawMandelbrot(Void)
	{
		Xmin = -2; Ymin = -2; Xmax = 2; Ymax = 2;

		int iter = (inStepModeToolStripMenuItem->Checked) ? 1 : 100;

		while (iter--)
		{
			for (int i = 0; i < panel1->Width; i++)
			{
				for (int j = 0; j < panel1->Height; j++)
				{
					PointD W(0, 0), C, WT;
					ToWorld(Point(i, j), C);

					for (int k = 0; k < NumIters; k++)
					{
						// преобразование точек
						WT.X = W.X * W.X - W.Y * W.Y + C.X;
						WT.Y = 2 * W.X * W.Y + C.Y;

						// выделение точек, не принадлежащих множеству
						if (WT.X * WT.X + WT.Y * WT.Y > 4)
						{
							Bmp->SetPixel(i, j, B->Color);
							break;
						}

						W.X = WT.X;
						W.Y = WT.Y;
					}

					// выделение точек, принадлежащих множеству
					if (WT.X * WT.X + WT.Y * WT.Y < 4)
						Bmp->SetPixel(i, j, P->Color);
				}

				//отрисовка изображения
				if ((inStepModeToolStripMenuItem->Checked) && (i % 3 == 0))
					G->DrawImage(Bmp, 0, 0);
			}

			if (continuousModeToolStripMenuItem->Checked)
			{
				G->DrawImage(Bmp, 0, 0);
				NumIters++;
			}
		}

		Xmin = -0.1; Ymin = -0.7; Xmax = 1.1; Ymax = 0.7;
	}

	/*// Процедура вычисления и отрисовки странного аттрактора Лоренца
	private: System::Void DrawLorenzAttractor(int plane)
	{
		type = ((plane == 0) ? LORENZ_ZX : LORENZ_XY);

		// начальные коэффициенты и координаты
		Double X = 3.051522, Y = 1.582542, Z = 15.62388, XT, YT, ZT;
		Double DT = 0.0001;
		int A = 5, B = 15, C = 1;

		int iters = 1000000;
		
		do
		{
			// преобразования точки
			X = XT = X + A * (-X + Y) * DT;
			Y = YT = Y + (B * X - Y - Z * X) * DT;
			Z = ZT = Z + (-C * Z + X * Y) * DT;

			Point PT(	(25.3*(((plane == 0)? X - Y * 0.292893 : Y - X * 0.292893)) + 320) - 60,	// X координата
						(-17 * (Z + X * 0.292893) + 392) + 60										// Y координата
					);
			// отрисовка изображения
			Bmp->SetPixel(PT.X, PT.Y, P->Color);
			if (iters % 250 == 0)
				G->DrawImage(Bmp, 0, 0);
		} while (--iters);
	}*/

	// Пункт меню для начала отрисовки выбранного фрактала
	private: System::Void drawToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		//this->ClearPanel();
		
		NumIters++;
		
		switch (type)
		{
			case CONSTRUCT:
				this->ClearPanel();
				this->DrawConstruct();
				break;
			case IFSD:
				this->DrawIFSD(NumIters, NumAffine, C, E0, SizePanel);
				break;
			case IFSR:
				this->ClearPanel();
				this->DrawIFSR();
				break;
			case MANDELBROT: 
				this->DrawMandelbrot();
				break;
			case JULIA:
				this->DrawJulia();
				break;
			/*case LORENZ_ZX:
				this->ClearPanel();
				this->DrawLorenzAttractor(0);
				break;
			case LORENZ_XY:
				this->ClearPanel();
				this->DrawLorenzAttractor(1);*/
		}
	}

	// Пример фрактала Минковского
	private: System::Void minkowskiFractalToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->ClearPanel();

		type = CONSTRUCT;
		
		OriginBase->Clear();
		OriginPattern->Clear();
		this->ClearBaseAndPattern();
		this->DefaultBaseAndPattern();
		
		this->DrawConstruct();
	}

	// Пример фрактала Серпинского (IFSD)
	private: System::Void sierpinskiFractalIFSDToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->ClearPanel();

		type = IFSD;

		delete C, E0;

		NumIters = 0;											// число итераций
		NumAffine = 3;											// число преобразований

		
		E0 = gcnew array<Byte, 2>(SizePanel, SizePanel);		// начальное множество E0

		// начальное множество - квадрат
		for (int i = 0; i < SizePanel; i++)
			for (int h = 0; h < SizePanel; h++)
				E0[i, h] = 1;

		C = gcnew array<Double, 2>(NumAffine, 6);				// матрица коэффициентов преобразований

		// Ковер Серпинского
		C[0, 0] = 0.5;		C[0, 1] = 0;		C[0, 2] = 0;		C[0, 3] = 0.5;		C[0, 4] = 0;		C[0, 5] = 0;
		C[1, 0] = 0.5;		C[1, 1] = 0;		C[1, 2] = 0;		C[1, 3] = 0.5;		C[1, 4] = 0.5;		C[1, 5] = 0;
		C[2, 0] = 0.5;		C[2, 1] = 0;		C[2, 2] = 0;		C[2, 3] = 0.5;		C[2, 4] = 0.25;		C[2, 5] = 0.43301;

		this->DrawIFSD(NumIters, NumAffine, C, E0, SizePanel);
	}

	// Пример фрактала Серпинского (IFSR)
	private: System::Void sierpinskiFractalIFSRToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->ClearPanel();
		type = IFSR;
		this->DrawIFSR();
	}

	// Пример фрактала Папоротник (IFSD)
	private: System::Void fernToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->ClearPanel();

		type = IFSD;

		delete C, E0;

		NumIters = 0;												// число итераций
		NumAffine = 4;												// число преобразований
		
		E0 = gcnew array<Byte, 2>(SizePanel, SizePanel);			// начальное множество E0

		// начальное множество - квадрат
		for (int i = 0; i < SizePanel; i++)
			for (int h = 0; h < SizePanel; h++)
				E0[i, h] = 1;

		C = gcnew array<Double, 2>(NumAffine, 6);					// матрица коэффициентов преобразований

		// Папоротник
		C[0, 0] = 0.7;		C[0, 1] = 0;		C[0, 2] = 0;		C[0, 3] = 0.7;		C[0, 4] = 0.1496;	C[0, 5] = 0.2962;
		C[1, 0] = 0.1;		C[1, 1] = -0.433;	C[1, 2] = 0.1732;	C[1, 3] = 0.25;		C[1, 4] = 0.4478;	C[1, 5] = 0.0014;
		C[2, 0] = 0.1;		C[2, 1] = 0.4333;	C[2, 2] = -0.1732;	C[2, 3] = 0.25;		C[2, 4] = 0.4445;	C[2, 5] = 0.1559;
		C[3, 0] = 0;		C[3, 1] = 0;		C[3, 2] = 0;		C[3, 3] = 0.3;		C[3, 4] = 0.4987;	C[3, 5] = 0.007;

		this->DrawIFSD(NumIters, NumAffine, C, E0, SizePanel);
	}

	// Пример фрактала Лист (IFSD)
	private: System::Void leafToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->ClearPanel();

		type = IFSD;

		delete C, E0;

		NumIters = 0;											// число итераций
		NumAffine = 2;											// число преобразований

		E0 = gcnew array<Byte, 2>(SizePanel, SizePanel);		// начальное множество E0

		// начальное множество - квадрат
		for (int i = 0; i < SizePanel; i++)
			for (int h = 0; h < SizePanel; h++)
				E0[i, h] = 1;

		C = gcnew array<Double, 2>(NumAffine, 6);				// матрица коэффициентов преобразований

		// Лист
		C[0, 0] = 0.4;		C[0, 1] = -0.373;	C[0, 2] = 0.06;		C[0, 3] = 0.6;		C[0, 4] = 0.353;	C[0, 5] = 0;
		C[1, 0] = -0.8;		C[1, 1] = 0.186;	C[1, 2] = 0.137;	C[1, 3] = 0.8;		C[1, 4] = 1.1;		C[1, 5] = 0.1;

		this->DrawIFSD(NumIters, NumAffine, C, E0, SizePanel);
	}

	// Пример фрактала Кристалл (IFSD)
	private: System::Void crystalToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->ClearPanel();

		type = IFSD;

		delete C, E0;

		NumIters = 0;											// число итераций
		NumAffine = 4;											// число преобразований
		
		E0 = gcnew array<Byte, 2>(SizePanel, SizePanel);		// начальное множество E0

		// начальное множество - квадрат
		for (int i = 0; i < SizePanel; i++)
			for (int h = 0; h < SizePanel; h++)
				E0[i, h] = 1;

		C = gcnew array<Double, 2>(NumAffine, 6);				// матрица коэффициентов преобразований

		// Кристалл
		C[0, 0] = 0.255;		C[0, 1] = 0;		C[0, 2] = 0;			C[0, 3] = 0.255;		C[0, 4] = 0.3726;		C[0, 5] = 0.6714;
		C[1, 0] = 0.255;		C[1, 1] = 0;		C[0, 2] = 0;			C[1, 3] = 0.255;		C[1, 4] = 0.1146;		C[1, 5] = 0.2232;
		C[2, 0] = 0.255;		C[2, 1] = 0;		C[0, 2] = 0;			C[2, 3] = 0.255;		C[2, 4] = 0.6306;		C[2, 5] = 0.2232;
		C[3, 0] = 0.370;		C[3, 1] = -0.642;	C[3, 2] = 0.642;		C[3, 3] = 0.370;		C[3, 4] = 0.6356;		C[3, 5] = -0.0061;

		this->DrawIFSD(NumIters, NumAffine, C, E0, SizePanel);
	}

	// Пример фрактала Мандельброта
	private: System::Void mandelbrotFractalToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e) 
	{
		this->ClearPanel();
		type = MANDELBROT;
		NumIters = 1;		// Число итераций
		this->DrawMandelbrot();
	}

	// Пример фрактала Жулиа
	private: System::Void juliaFractalToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		this->ClearPanel();
		type = JULIA;
		NumIters = 1;		// Число итераций
		this->DrawJulia();
	}

	// Пример странного аттрактора Лоренца (Z-X плоскость)
	private: System::Void lorenzSystemZXToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		//this->ClearPanel();
		//type = LORENZ_ZX;
		//this->DrawLorenzAttractor(0);
	}
	
	// Пример странного аттрактора Лоренца (X-Y плоскость)
	private: System::Void lorenzSystemXYToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		//this->ClearPanel();
		//type = LORENZ_XY;
		//this->DrawLorenzAttractor(1);
	}

	// Сохранение полученного рисунка
	private: System::Void saveToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			Bmp->Save(saveFileDialog1->FileName, System::Drawing::Imaging::ImageFormat::Jpeg);
	}
	
	// Режим отрисовки по шагам
	private: System::Void inStepModeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		NumIters = 0;
		inStepModeToolStripMenuItem->Checked = true;
		continuousModeToolStripMenuItem->Checked = false;
	}

	// Режим непрерывной отрисовки
	private: System::Void continuousModeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		NumIters = 0;
		continuousModeToolStripMenuItem->Checked = true;
		inStepModeToolStripMenuItem->Checked = false;
	}

	// Выбор цвета кисти
	private: System::Void settingsToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		colorDialog1->ShowDialog();
		P-> Color = colorDialog1-> Color;
	}

	// Выбор цвета заднего плана
	private: System::Void selectBackgroundColorToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		colorDialog1->ShowDialog();
		B -> Color = colorDialog1-> Color;
	}
	
	// Перерисовка изображения в случае, если часть окна выходит за границы экрана
	private: System::Void MainForm_Paint(
		System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		G->DrawImage(Bmp, 0, 0);
	}

	// Меню "О программе"
	private: System::Void aboutToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		delete AboutBox;
		AboutBox = gcnew Form();

		AboutBox->Name = L"AboutBox";
		AboutBox->Text = L"About";
		AboutBox->ClientSize = System::Drawing::Size(300, 50);
		AboutBox->MaximizeBox = false;
		AboutBox->Icon = this->Icon;
		AboutBox->ShowInTaskbar = false;
		AboutBox->TopMost = true;
		AboutBox->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
		AboutBox->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;

		Label ^LabelAboutBox = gcnew Label();
		AboutBox->Controls->Add(LabelAboutBox);

		LabelAboutBox->Text = L"\n     FractalViewer. Copyright (C) 2020 Kuzmichev Nikolay\n     This software protected by The BSD 3-Clause License";
		LabelAboutBox->ClientSize = System::Drawing::Size(300, 50);
		LabelAboutBox->Visible = true;
		
		AboutBox->Visible = true;
	}

	// Загрузка параметров фрактала из файла (только конструктивные фракталы, с базой и шаблоном)
	private: System::Void openFileToolStripMenuItem_Click(
		System::Object^  sender, System::EventArgs^  e)
	{
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			OriginBase->Clear();
			OriginPattern->Clear();

			type = CONSTRUCT;

			DataIn = System::IO::File::OpenText(openFileDialog1->FileName);
			array<Char> ^separator = gcnew array<Char>{' '};

			while ((DataStr = DataIn->ReadLine()) != nullptr)
				if (DataStr != "")
				{
					delete result;
					result = DataStr->Split(separator);

					for (int iter = 0; iter < result->Length; iter++)
					{
						if (result[iter] == L"PP")
						{
							String^ PatternX = result[++iter];
							String^ PatternY = result[++iter];

							OriginPattern->Add(PointD(Convert::ToDouble(PatternX), Convert::ToDouble(PatternY)));
							continue;
						}
						if (result[iter] == L"BP")
						{
							String^ BaseX = result[++iter];
							String^ BaseY = result[++iter];

							OriginBase->Add(PointD(Convert::ToDouble(BaseX), Convert::ToDouble(BaseY)));
							continue;
						}
					}
				}

			this->ClearBaseAndPattern();
			this->LoadOriginBaseAndPattern();

			delete DataStr;
			DataIn->Close();
			delete DataIn;
		}
	}
};
}