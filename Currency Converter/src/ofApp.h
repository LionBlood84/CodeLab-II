#pragma once

#include "json.hpp"
#include "ofMain.h"

using json = nlohmann::json;

enum class ActiveInput {
	NONE,
	AMOUNT
};

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;
	void mousePressed(int x, int y, int button) override;
	void urlResponse(ofHttpResponse & response);

	void triggerConversion();
	void loadApiKey();
	bool isNumericString(const std::string & str);

	std::string apiKey;
	std::string amountText;
	ActiveInput activeField;

	std::vector<std::string> currencyList;
	int baseCurrencyIndex;
	int targetCurrencyIndex;

	ofRectangle amountBox;
	ofRectangle basePrevBtn;
	ofRectangle baseNextBtn;
	ofRectangle targetPrevBtn;
	ofRectangle targetNextBtn;
	ofRectangle convertBtn;

	std::string statusMessage;
	std::string convertedResult;
	std::string exchangeRateInfo;
	bool isLoading;

	ofTrueTypeFont fontTitle;
	ofTrueTypeFont fontHeader;
	ofTrueTypeFont fontLarge;
	ofTrueTypeFont fontBody;
	bool hasCustomFont;
};
