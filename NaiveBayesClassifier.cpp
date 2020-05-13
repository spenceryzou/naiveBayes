#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iomanip>
using namespace std;

int main(int argc, char* argv[]){
    auto start = chrono::high_resolution_clock::now(); 
    unordered_map<string, int> positiveWordBag;
    unordered_map<string, int> negativeWordBag;
    int positiveLineCount = 0;
    int negativeLineCount = 0;
    int positiveWordCount = 0;
    int negativeWordCount = 0;
    ifstream trainingInput;
    string line;
    string word;
    int actualClass;
    //read training data into word bags
    trainingInput.open(argv[1]);
    while(getline(trainingInput, line, ',')){
        trainingInput >> actualClass;
        stringstream ss(line);
        if(actualClass == 1){
            positiveLineCount += 1;
            while(ss >> word){
                if(word == "i"){
                    continue;
                }else{
                 positiveWordBag[word]+=1;
                    positiveWordCount += 1;
                }
            } 
        }else{
            negativeLineCount += 1;
            while(ss >> word){
                if(word == "i"){
                    continue;
                }else{
                negativeWordBag[word]+=1;
                    negativeWordCount += 1;
                }
            } 
        }
    }
    trainingInput.close();
    //cout << positiveLineCount << " " << negativeLineCount;
    //calculate probabilities
    double probPositive = (double)positiveLineCount/(positiveLineCount+negativeLineCount);
    double probNegative = (double)negativeLineCount/(positiveLineCount+negativeLineCount);
    //apply classifier on training dataset
    int correctPrediction = 0;
    int predictedClass;
    trainingInput.open(argv[1]);
    while(getline(trainingInput, line, ',')){
        double probPositivePrediction = log(probPositive);
        double probNegativePrediction = log(probNegative);
        trainingInput >> actualClass;
        stringstream ss(line);
        while(ss >> word){
            //n of k is number of words in a specific class, n is the total number of words accounting for duplicates in the specific class.
            probPositivePrediction+=log(((double)(positiveWordBag[word]+1))/(double)(positiveWordCount+1));
            probNegativePrediction+=log(((double)(negativeWordBag[word]+1))/(double)(negativeWordCount+1));
        } 
        if(max(probPositivePrediction,probNegativePrediction)==probPositivePrediction)
            predictedClass = 1;
        else
            predictedClass = 0;
        if(predictedClass == actualClass)
            correctPrediction += 1;
    }
    double trainingAccuracy = ((double)correctPrediction)/(positiveLineCount+negativeLineCount);
    auto stop = chrono::high_resolution_clock::now(); 
    auto trainDuration = chrono::duration_cast<std::chrono::seconds>( stop - start ).count();
    //apply classifier on testing dataset
    start = chrono::high_resolution_clock::now(); 
    ifstream testingInput;
    testingInput.open(argv[2]);
    correctPrediction = 0;
    int testLineCount = 0;
    while(getline(testingInput, line, ',')){
        testLineCount++;
        double probPositivePrediction = log(probPositive);
        double probNegativePrediction = log(probNegative);
        testingInput >> actualClass;
        stringstream ss(line);
        while(ss >> word){
            //n of k is number of words in a specific class, n is the total number of words accounting for duplicates in the specific class.
            probPositivePrediction+=log(((double)(positiveWordBag[word]+1))/(double)(positiveWordCount+1));
            probNegativePrediction+=log(((double)(negativeWordBag[word]+1))/(double)(negativeWordCount+1));
        } 
        if(max(probPositivePrediction,probNegativePrediction)==probPositivePrediction)
            predictedClass = 1;
        else
            predictedClass = 0;
        cout << predictedClass << endl;
        if(predictedClass == actualClass)
            correctPrediction += 1;
    }
    double testingAccuracy = ((double)correctPrediction)/(testLineCount);
    stop = chrono::high_resolution_clock::now(); 
    auto testDuration = chrono::duration_cast<std::chrono::seconds>( stop - start ).count();
    cout << trainDuration << " seconds (training)" << endl;
    cout << testDuration << " seconds (labeling)" << endl;
    cout << fixed; 
    cout << setprecision(3) << trainingAccuracy << " (training)" << endl;
    cout << setprecision(3) << testingAccuracy << " (testing)" << endl;
}