#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include "global.hpp"
int key = HELP_STR.size() + 30;

std::string enc(std::string inpString) 
{ 
    // Define XOR key 
    // Any character value will work 
  
    // calculate length of input string 
    int len = inpString.size();
  
    // perform XOR operation of key 
    // with every caracter in string 
    for (int i = 0; i < len; i++) 
    { 
        inpString[i] = inpString[i] ^ key; 
    } 
	 return inpString;
} 

bool update_highscore(int points) {
   std::ofstream filew;
   std::ifstream filer;
   int hspoints;
   bool newfile = false;
	
   filer.open("./highscores", std::ios::out);
	bool hs = false; //This variable represents if the given points results in a new highscore or not
	   if (!filer.is_open()) {
		filew.open("./highscores");
		filew << enc("0");
		hspoints = 0;
		newfile = true;
		filew.close();
	}
		if (!newfile) {
			std::string output = "";
			std::string line;
	   
		while (std::getline(filer, line)) {
			hspoints = std::stoi(enc(line));
		}
		}

	   filer.close();
	   
	   if (hspoints > points) {
		   hs = false;
	   } else {
		   filew.open("./highscores");
		   std::string newpoints = std::to_string(points);
		   filew << enc(newpoints);
		   filew.close();
		   hs = true;
	   }
	   return hs;
	   
   

}

int get_highscore() {
	std::ofstream filew;
	std::ifstream filer;
	int hspoints;
	bool newfile = false;
	filer.open("./highscores", std::ios::out);
	   if (!filer.is_open()) {
		filew.open("./highscores");
		filew << enc("0");
		hspoints = 0;
		newfile = true;
		filew.close();
	}
		if (!newfile) {
			std::string output = "";
			std::string line;
	   
		while (std::getline(filer, line)) {
			hspoints = stoi(enc(line));
		}
		}

	   filer.close();

	   return hspoints;
	   
}