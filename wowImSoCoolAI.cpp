#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <cmath>
#include <fstream>
#include <iterator>
#include <string>
#include <tuple>
#include <ioManip>
#include <windows.h>
#define GetCurrentDir _getcwd
using namespace std;
typedef long long ll;
string outFileName = "C:\\Users\\Antho\\source\\repos\\wowImSoCoolAI\\nOut.txt";
string inFileName = "C:\\Users\\Antho\\source\\repos\\wowImSoCoolAI\\nOut.txt";

char toGraphic(float a, float min, float max) {
	float avg = (max + min) / 2;

	if (a > (max * 3 + min) / 4)return (char)178;
	if (a > (max + min) / 2)return (char)177;
	if (a > (max + min * 3) / 4)return (char)176;
	return ' ';

}


int ReverseInt(int i)
{
	unsigned char ch1, ch2, ch3, ch4;
	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;
	return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}
void ReadMNIST(int NumberOfImages, int DataOfAnImage, vector<vector<float>>& arr)
{
	arr.resize(NumberOfImages, vector<float>(DataOfAnImage));
	ifstream file("C:\\Users\\Antho\\source\\repos\\wowImSoCoolAI\\train-images-idx3-ubyte.gz", ios::binary);
	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;
		file.read((char*)& magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		file.read((char*)& number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		file.read((char*)& n_rows, sizeof(n_rows));
		n_rows = ReverseInt(n_rows);
		file.read((char*)& n_cols, sizeof(n_cols));
		n_cols = ReverseInt(n_cols);
		number_of_images = NumberOfImages;
		for (int i = 0; i < number_of_images; ++i)
		{
			for (int r = 0; r < n_rows; ++r)
			{
				for (int c = 0; c < n_cols; ++c)
				{
					unsigned char temp = 0;
					file.read((char*)& temp, sizeof(temp));
					arr[i][(n_rows * r) + c] = (float)temp / 256;
					//cout << toGraphic((float)temp / 256, 0, 1) ;
				}
				//cout << '\n';
			}
			if (i % 5000 == 0)cout << i << '\n';
			//cout << "\n\n";
		}
	}
	file.close();
}

void ReadLabels(int NumberOfLabels, vector<float>& arr)
{
	arr.resize(NumberOfLabels, 1);
	ifstream file("C:\\Users\\Antho\\source\\repos\\wowImSoCoolAI\\train-labels-idx1-ubyte.gz", ios::binary);
	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;
		file.read((char*)& magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		file.read((char*)& number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		number_of_images = NumberOfLabels;
		for (int i = 0; i < number_of_images; ++i)
		{
			unsigned char temp = 0;
			file.read((char*)& temp, sizeof(temp));
			arr[i] = (float)temp;
			//cout << (float)temp << ' ';

		}
	}
	file.close();
}


typedef unsigned int uint;
typedef vector<int> vi;
typedef vector<float> vf;
typedef vector<double> vd;

typedef vector<vector<float>> vvf;
typedef vector<vector<double>> vvd;

typedef vector<vector<vector<float>>> vvvf;
typedef vector<vector<vector<double>>> vvvd;



vi layerSizes;
vvvf weights;
vvf neurons;
vvf neurons2;
vvf bias;

vvvf gWeights;
vvvd iWei;

vvf gBias;
vvf iBias;
float lRate;
int numPics = 200;
int epoches = 30;
int bSize = 64;
int inSize = 784;
int outSize = 10;

float cust = 10;


int theTime;
int startTime;
long long totalSteps;


void initWB() {

	weights.clear();
	bias.clear();
	for (int i = 0; i < layerSizes.size() - 1; i++) {
		vvf jWei;
		vvd ijWei;

		vf jBias;
		vf ijBias;
		ijBias.resize(layerSizes[i + 1]);


		for (int j = 0; j < layerSizes[i + 1]; j++) {
			vf kWei;
			vd ikWei;
			ikWei.resize(layerSizes[i],0);
			for (int k = 0; k < layerSizes[i]; k++) {
				kWei.push_back((float(rand() % 400) - 200) / 100);
			}
			jWei.push_back(kWei);
			ijWei.push_back(ikWei);
			jBias.push_back((float(rand() % 400) - 200) / 800);

		}
		iWei.push_back(ijWei);
		iBias.push_back(ijBias);
		weights.push_back(jWei);
		bias.push_back(jBias);

	}
}


void seeWB() {
	for (int i = 0; i < weights.size(); i++) {
		for (int j = 0; j < weights[i].size(); j++) {
			for (int k = 0; k < weights[i][j].size(); k++) {
				cout << toGraphic(weights[i][j][k], -1, 1);
				if (k % 28 == 0 && i == 0)cout << '\n';
			}cout << toGraphic(bias[i][j], -1.5, 1.5) << '\n';
		}cout << '\n';
	}
}

void loadWB() {
	ifstream file;
	string tsh;
	file.open(inFileName);
	if (file.is_open()) {
		float a;

		string word;

		for (uint i = 0; i < weights.size(); i++) {
			for (uint j = 0; j < weights[i].size(); j++) {
				for (uint k = 0; k < weights[i][j].size(); k++) {
					file >> a;
					weights[i][j][k] = a;
					//cout << toGraphic(weights[i][j][k], -0.3, 0.3);
					//if (k % 28 == 0 && i == 0)cout << '\n';
				}
				float b;
				file >> b;
				bias[i][j] = b;  //cout << "Bias " << toGraphic(bias[i][j], -2, 2) << '\n';
			}//cout << i << '\n';
		}

	}
	file.close();


}

void saveWB() {
	ofstream output_file(outFileName);
	for (int i = 0; i < weights.size(); i++) {
		for (int j = 0; j < weights[i].size(); j++) {
			for (int k = 0; k < weights[i][j].size(); k++) {
				output_file << setprecision(6) << weights[i][j][k] << ' ';
			}output_file << setprecision(6) << bias[i][j] << '\n';
		}output_file << '\n';
	}
}



float calcCost(vf correct, vf actual) {
	float ans = 0;
	for (int i = 0; i < actual.size(); i++) {
		float diff = actual[i] - correct[i];
		ans += diff * diff;
	}
	return ans;
}
float actFunc(float x) {
	return 1 / (1 + exp((float)-x));
}

float dActFunc(float x) {
	float y = actFunc(x);
	return y * (1.0 - y);
}



pair<vf, float> calcGradient(vf input, vf result) {
	totalSteps++;
	neurons.clear();
	neurons2.clear();

	neurons.push_back(input);
	neurons2.push_back(input);

	for (int i = 0; i < weights.size(); i++) {
		vf nextNeuralLayer;
		vf nextNeuralLayer2;

		for (int j = 0; j < weights[i].size(); j++) {
			float nextNeuron = 0;

			for (int k = 0; k < weights[i][j].size(); k++) {
				nextNeuron += weights[i][j][k] * neurons[i][k];
			}
			nextNeuron += bias[i][j];
			nextNeuralLayer2.push_back(nextNeuron);
			nextNeuron = actFunc(nextNeuron);
			nextNeuralLayer.push_back(nextNeuron);
		}
		neurons.push_back(nextNeuralLayer);
		neurons2.push_back(nextNeuralLayer2);

	}

	vf ans = neurons.back();
	vf activ;
	for (int q = 0; q < ans.size(); q++) {
		activ.push_back((ans[q] - result[q]) * 2);
	}
	vf nextActiv = activ;
	for (int i = weights.size() - 1; i > -1; i--) {
		activ = nextActiv;
		nextActiv.clear();
		nextActiv.resize(neurons[i].size());
		for (int j = 0; j < weights[i].size(); j++) {
			float na = activ[j] * dActFunc(neurons2[i + 1][j]);
			for (int k = 0; k < weights[i][j].size(); k++) {
				float w = weights[i][j][k] + (float(rand() % 400) - 200) / 200000;
				float n2 = neurons[i][k];
				nextActiv[k] += (w * na);// +(float(rand() % 400) - 200) / 2000;
				float c = (n2 * na * lRate);
				iWei[i][j][k] = iWei[i][j][k]*0.9 + (double)c;
				//if (totalSteps % 1000 == 0) {
				gWeights[i][j][k] -= (c) + iWei[i][j][k]*0.003;
				///if (totalSteps % 999 == 0) {
				///	//cout << fabs(iWei[i][j][k])<<' ';
				///	if (fabs(iWei[i][j][k]) < 0.00001) {
				///		gWeights[i][j][k]=0;
				///	}
				///	else {
				///		gWeights[i][j][k] -= (0.000005 / iWei[i][j][k]);
				///	}
				///}

			}
			//if (totalSteps % 1000 == 0)cout << '\n';
			float b = (na * lRate);
			//iBias[i][j] = iBias[i][j] * 0.999 + b*0.1;
			gBias[i][j] -= (b * lRate * 1.3);
			//if (cust < 0.85)cout << '\n';
		}
		//if (totalSteps % 1000 == 0)cout << '\n';
		//if (cust < 0.85)cout << '\n';

	}			//if (cust < 0.85)cout << '\n';


	return { ans, calcCost(result, ans) };
}




void addLayers(int numLayers, int size) {
	for (int i = 0; i < numLayers; i++) {
		layerSizes.push_back(size);
	}
}
void sayTime(float seconds) {

	float hours, minutes;
	minutes = seconds / 60;
	hours = minutes / 60;
	cout << int(hours) << ":" << int((int)minutes % 60)
		<< ":" << setprecision(2) << float((int)seconds % 60) + (float)(int)seconds - seconds;
}
void timeStuff(float perN, float finishedN, float ns, float nsDone) {
	//cout << endl << "Time:" << time(NULL) << "THETIME " << theTime << endl;

	int calcPerSec = (float)totalSteps / ((float)time(NULL) - (float)startTime);

	//cout << "Calculations Per Sec " << thisSec<<endl;
	//cout << iter << ' ' << (numPics * 0.72)*numNetworks << '\n';
	float nextNet = (perN - finishedN);
	float nextEp = (ns - nsDone) * perN;
	cout << "Time to next Batch: "; sayTime((nextNet) / (float)calcPerSec);
	cout << " Time to finish: "; sayTime((nextEp + nextNet) / (float)calcPerSec);
	cout << " Best Cost " << setprecision(7) << cust << ' ';
	//cout << " " << thisSec;


	//thisSec = 0;
	theTime = time(NULL);


}
void updatelRate(float itr) {
	float max = 0.1;
	float base = 0.001;
	float stepSize = 2000;
	if (itr < 1)itr = 1;
	lRate = (max - base) * powf(100, -fmod(2 * itr / stepSize, 2) + 3) / 1000000 + base;
	//https://www.desmos.com/calculator/ko1bybx5a7
}
vf ansVec(int num) {
	vf res(10);
	res[(uint)num] = 1;
	return res;
}
uint brand() {
	return (rand() * rand() + rand());
}
int main()
{
	srand(time(NULL));

	numPics = 60000;

	vvf ar;
	vf lb;

	ReadLabels(numPics, lb);
	ReadMNIST(numPics, inSize, ar);
	addLayers(1, inSize);
	addLayers(1, 40);
	addLayers(1, 20);
	addLayers(1, outSize);

	startTime = time(NULL);
	theTime = startTime;
	totalSteps = 0;
	//lRate = 0.05;
	epoches = 16900;
	bSize = 32;

	int uSpd = 10;

	initWB();
	//loadWB();
	//seeWB();
	int dont = 0;
	gWeights = weights;
	gBias = bias;
	tuple <vvvf, vvf, float> bestNet = make_tuple(weights, bias, 10);
	cout << "training starting\n";
	int modelSize = 0;
	for (int i = 0; i < layerSizes.size()-1; i++) {
		modelSize += layerSizes[i] * layerSizes[i + 1];
	}
	cout << modelSize<<" Parameter model\n";
	for (int e = 0; e < epoches; e++) {
		float cost = 0; int q = 0; int cor = 0;
		while (q < bSize) {
			uint i = brand() % (int)(numPics * 0.9);
			while (i == dont)i = brand() % (int)(numPics * 0.9);
			pair<vf, float> u = calcGradient(ar[i], ansVec(lb[i]));
			cost += u.second; vf g = u.first;
			//if (u.second < get<2>(bestNet)&& u.second>0.3)bestNet = make_tuple(weights, bias, u.second);
			q++;
			int netAns = distance(g.begin(), max_element(g.begin(), g.end()));
			cor += (netAns == lb[i] ? 1 : 0);
			dont = netAns;
			//for (int x = 0; x < 2; x++) {
			//	if (netAns != lb[i]) {
			//		u = calcGradient(ar[i], ansVec(lb[i]));
			//		cost += u.second; g = u.first;
			//		//q++;
			//		netAns = distance(g.begin(), max_element(g.begin(), g.end()));
			//		cor += (netAns == lb[i] ? 1 : 0);
			//	}
			//	else break;
			//}
			//if (totalSteps % 1000 == 0) {
			//	if()bestNet = make_tuple(weights, bias, 10);
			//}
			//if (totalSteps % 200 == 0 ) {
			//	weights = get<0>(bestNet);
			//	bias = get<1>(bestNet);
			//	cout << "reset\n";
			//}
			updatelRate(totalSteps);
			if (time(NULL) > theTime) {
				cout << e << ' ';
				cust = (float)cost / (float)q;
				timeStuff(bSize, q + 1, epoches, e + 1);
				cout << (float)cor / (float)q * 100 << "% Accuracy\n";
				cout << lRate << '\n';

			}
		}
		if ((float)cor / (float)q > 0.9)break;
		weights = gWeights;
		bias = gBias;
	}
	
	cout << "Finished training in ";
	sayTime((float)time(NULL) - (float)startTime);
	float cost = 0; int q = 0; int cor = 0;
	int wEx = 0;
	while (q < 500) {
		ll i = (rand() * rand() + rand()) % (int)(numPics * 0.9);
		vf a = ar[i];
		vf b(10);
		b[lb[i]] = 1;
		pair<vf, float> u = calcGradient(a, b);
		cost += u.second;
		vf g = u.first;
		totalSteps++; q++;
		int out = distance(g.begin(), max_element(g.begin(), g.end()));
		cor += out == lb[i] ? 1 : 0;
		if (/*out != lb[i] &&*/ wEx < 60) {
			cout << "What the image was: ";
			for (int n = 0; n < a.size(); n++) { if (n% 28 == 0)cout << '\n'; cout << toGraphic(a[n], 0, 1) << toGraphic(a[n], 0, 1); }
			cout << "\nWhat the network said: " << out << "\nWhat it really was: " << lb[i];
			cout << "\nCost: " << u.second << "\n\n\n";
			wEx++;
		}
		else if (wEx > 19&&q%20==0) {
			cout << q << " out of " << 500 << " Validation\n";
		}
		cust = (float)cost / (float)q;
	}
	seeWB();
	cout << "Average Cost: " << cust << '\n';
	cout << (float)cor / (float)q * 100 << "% Accuracy\n";
	if ((float)cor / (float)q * 100 > 93.5) { saveWB(); cout << "Auto Saved network to " << outFileName; }
	else {
		cout << "Save Network? (y/n)";
	BEFORE:
		char g;
		cin >> g;
		if (g == 'y') {
			saveWB();
			cout << "\nSaved Network to " << outFileName;
		}
		else if (g == 'n') {
			cout << "\nDiscarded Network";

		}
		else {
			cout << "\nNot y or n. Try again";
			goto BEFORE;
		}
	}
}