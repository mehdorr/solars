#include "MyForm.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace std;

struct Information {
	double result = 0;
};

int rowNumberCheck(string fileName) {
	string temp;
	int rowCounter = 0;
	fstream checkFile(fileName, std::ios_base::in);
	while (checkFile >> temp)
	{
		rowCounter++;
	}
	checkFile.close();
	return rowCounter;
}

void fileReader(string fileName, System::Windows::Forms::Label^ labelAvg, System::Windows::Forms::Label^ labelMin, System::Windows::Forms::Label^ labelMax, System::Windows::Forms::DataVisualization::Charting::Chart^ chart) {
	int counter = 0;
	double averageEM = 0, minEM = 9999200, maxEM = 0, averaget1 = 0;
	string *rowContent;
	rowContent = new string[rowNumberCheck(fileName)];

	Information *result;
	result = new Information[rowNumberCheck(fileName)];

	fstream log("log.txt", std::ios::app);
	fstream file(fileName, std::ios_base::in);
	while (file >> rowContent[counter])
	{
		istringstream iss(rowContent[counter]);
		vector<string> tokensEM;
		string tokenEM;
		while (getline(iss, tokenEM, ':')) {
			if (!tokenEM.empty())
				tokensEM.push_back(tokenEM);
		}

		log << "Date: " << tokensEM[0] << "/" << tokensEM[1] << "/" << tokensEM[2] << " Time: " << tokensEM[3] << ":" << tokensEM[4] << endl;
		log << "t1: " << tokensEM[6] << " | t2: " << tokensEM[8] << " | t3: " << tokensEM[10] << " | t4: " << tokensEM[12] << endl;
		log << "p: " << tokensEM[14] << " | em: " << tokensEM[16] << endl;
		log << "e1: " << tokensEM[18] << " | e2: " << tokensEM[20] << " | e3: " << tokensEM[22] << " | e4: " << tokensEM[24] << endl << endl;

		double tokenTemp = stod(tokensEM[16]);
		chart->Series["valEM"]->Points->AddY(tokenTemp);

		result[counter].result = stod(tokensEM[16]);
		averageEM += result[counter].result;
		if (minEM > result[counter].result) {
			minEM = result[counter].result;
		}
		if (maxEM < result[counter].result) {
			maxEM = result[counter].result;
		}
		counter++;
	}
	log << "END OF CALCULATION SERIES......................" << endl << endl << endl << endl << endl;
	log.close();
	file.close();
	delete[] result;
	averageEM = averageEM / rowNumberCheck(fileName);

	labelAvg->Text = Convert::ToString(averageEM);
	for (int counterAvg = counter; counterAvg > 0; counterAvg--) {
		chart->Series["avgEM"]->Points->AddY(averageEM);
	}
	labelMin->Text = Convert::ToString(minEM);
	for (int counterMin = counter; counterMin > 0; counterMin--) {
		chart->Series["minEM"]->Points->AddY(minEM);
	}
	labelMax->Text = Convert::ToString(maxEM);
	for (int counterMax = counter; counterMax > 0; counterMax--) {
		chart->Series["maxEM"]->Points->AddY(maxEM);
	}
}

void solar::MyForm::run(System::Windows::Forms::TextBox^ tBox) {
	//create a managed system string
	String^ fileNameManaged = "test";
	if (tBox->Text == "") {
		MessageBox::Show("Not a valid file name! Please enter a valid file name!");
	}
	else {
		fileNameManaged = tBox->Text + ".txt";
	}

	//convert the system string to std::string
	string fileNameUnmanaged = msclr::interop::marshal_as<std::string>(fileNameManaged);

	fileReader(fileNameUnmanaged, avgEM, minEM, maxEM, chartVals);

}

[STAThread]
void Main(cli::array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	solar::MyForm form;
	Application::Run(%form);
}