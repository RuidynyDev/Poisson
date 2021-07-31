//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
//-----------------------------------------------------входные данные
	int n = Edit1->Text.ToInt();
	double edge = Edit2->Text.ToDouble();
	double omega = Edit3->Text.ToDouble();
	double start = Edit4->Text.ToDouble();
	double d = Edit5->Text.ToDouble();
	bool choice = true;   		//по умолчанию - уравнение Пуассона
	if (ComboBox1->ItemIndex == 0)
		choice = false; 	//выбрано уравнение Лапласа

//------------------------------------------------------------расчеты
	U *u = new U (n, edge, start, omega, d, choice);
//-------------------------------------------------сохранить результаты
	save_file = new TSaveDialog(Form1);
	save_file->Filter = "Text files (*.txt)|*.txt|"
						"All Files";
	save_file->FilterIndex = 1;

	if (save_file->Execute())
	{
		UnicodeString file_name = save_file->FileName;
		TTextWriter *text_writer = new TStreamWriter (file_name, false,
									TEncoding::UTF8, 1024);
		UnicodeString s = "Poisson";
		if (!choice)
			s = "Laplas";
		text_writer->WriteLine(s);
		text_writer->WriteLine("N = " + UnicodeString(n));
		text_writer->WriteLine("L = " + UnicodeString(edge));
		text_writer->WriteLine("Omega = " + UnicodeString(omega));
		text_writer->WriteLine("Precision = " + UnicodeString(d));
		text_writer->WriteLine("Number of iterations = " + UnicodeString(u->iteration));
		s = "Precision not achieved";
		if (u->done)
			s = "Precision achieved";
		text_writer->WriteLine(s);
		text_writer->WriteLine();
		s = "U(x,y,z) values";
		text_writer->WriteLine (s);

		double h = 1.0 * edge / n;
		for (int i = 0; i < n+1; i++)
		{
			text_writer->WriteLine("x = " + UnicodeString(i*h));
			for (int j = 0; j < n+1; j++)
			{
				for (int k = 0; k < n+1; k++)
				{
					 text_writer->Write(FormatFloat("0.000", u->distribution[i][j][k]) + "\t");
				}
				text_writer->WriteLine();
			}
			text_writer->WriteLine();
			text_writer->WriteLine();
		}

		text_writer->Close();
		delete text_writer;
		text_writer = NULL;
		ShowMessage ("The results are successfully written into " + file_name);
	}

	delete u;
	u = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	delete save_file;
	save_file = NULL;
}
//---------------------------------------------------------------------------
