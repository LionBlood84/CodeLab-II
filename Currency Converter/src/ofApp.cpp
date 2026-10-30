#include "ofApp.h"

void ofApp::setup() {
	ofSetWindowShape(1280, 800);
	ofSetWindowTitle("Global Currency Converter - FX Intelligence");
	ofSetBackgroundColor(20, 24, 33);

	ofRegisterURLNotification(this);
	loadApiKey();

	currencyList = {
		"USD", "EUR", "GBP", "JPY", "CAD",
		"AUD", "CHF", "CNY", "INR", "SGD"
	};

	baseCurrencyIndex = 0;
	targetCurrencyIndex = 1;
	amountText = "100.00";
	activeField = ActiveInput::NONE;
	isLoading = false;

	amountBox.set(60, 160, 280, 50);
	basePrevBtn.set(370, 160, 45, 50);
	baseNextBtn.set(535, 160, 45, 50);
	targetPrevBtn.set(610, 160, 45, 50);
	targetNextBtn.set(775, 160, 45, 50);
	convertBtn.set(850, 160, 220, 50);

	statusMessage = "Ready. Configure currencies and amount, then convert.";
	convertedResult = "--";
	exchangeRateInfo = "";

	hasCustomFont = fontBody.load(OF_TTF_SANS, 14);
	if (hasCustomFont) {
		fontTitle.load(OF_TTF_SANS, 22);
		fontHeader.load(OF_TTF_SANS, 16);
		fontLarge.load(OF_TTF_SANS, 32);
	}
}

void ofApp::loadApiKey() {
	ofBuffer buffer = ofBufferFromFile("api_key.txt");
	if (buffer.size() > 0) {
		apiKey = ofTrim(buffer.getText());
	} else {
		apiKey = "fca_live_E8zTMfpEzAbntf3t1Og3Py17XULXaeP9IQJJBP1A";
	}
}

bool ofApp::isNumericString(const std::string & str) {
	if (str.empty()) return false;
	int dotCount = 0;
	for (char c : str) {
		if (c == '.') {
			dotCount++;
			if (dotCount > 1) return false;
		} else if (!std::isdigit(static_cast<unsigned char>(c))) {
			return false;
		}
	}
	return true;
}

void ofApp::triggerConversion() {
	std::string rawAmount = ofTrim(amountText);

	if (rawAmount.empty()) {
		statusMessage = "Input validation failed: Amount field cannot be empty.";
		convertedResult = "--";
		return;
	}

	if (!isNumericString(rawAmount)) {
		statusMessage = "Input validation failed: Amount must be a valid numerical value.";
		convertedResult = "--";
		return;
	}

	float value = ofToFloat(rawAmount);
	if (value < 0.0f) {
		statusMessage = "Input validation failed: Amount cannot be negative.";
		convertedResult = "--";
		return;
	}

	std::string base = currencyList[baseCurrencyIndex];
	std::string target = currencyList[targetCurrencyIndex];

	if (base == target) {
		convertedResult = ofToString(value, 2) + " " + target;
		exchangeRateInfo = "1 " + base + " = 1.0000 " + target;
		statusMessage = "Identical currencies selected. Direct parity calculated.";
		return;
	}

	isLoading = true;
	statusMessage = "Fetching live foreign exchange rates from FreeCurrencyAPI...";

	std::string url = "https://api.freecurrencyapi.com/v1/latest?apikey=" + apiKey + "&base_currency=" + base + "&currencies=" + target;
	ofLoadURLAsync(url, "fx_convert");
}

void ofApp::urlResponse(ofHttpResponse & response) {
	isLoading = false;

	if (response.status != 200) {
		std::string serverErr = "";
		try {
			json errJson = json::parse(response.data.getText());
			if (errJson.contains("message")) {
				serverErr = " - " + errJson["message"].get<std::string>();
			}
		} catch (...) { }
		statusMessage = "API Error (HTTP " + ofToString(response.status) + ")" + serverErr;
		return;
	}

	try {
		json data = json::parse(response.data.getText());

		if (data.contains("data") && data["data"].is_object()) {
			std::string target = currencyList[targetCurrencyIndex];
			std::string base = currencyList[baseCurrencyIndex];

			if (data["data"].contains(target)) {
				double rate = data["data"][target].get<double>();
				double amount = ofToDouble(amountText);
				double total = amount * rate;

				convertedResult = ofToString(total, 2) + " " + target;
				exchangeRateInfo = "1 " + base + " = " + ofToString(rate, 4) + " " + target;
				statusMessage = "Conversion successfully completed via live market rate.";
			} else {
				statusMessage = "API Error: Requested currency not found in payload.";
			}
		} else {
			statusMessage = "API Error: Invalid payload structure received.";
		}
	} catch (std::exception & e) {
		statusMessage = "Parser exception: Malformed JSON returned.";
	}
}

void ofApp::update() { }

void ofApp::draw() {
	ofSetColor(240, 245, 255);
	if (hasCustomFont) {
		fontTitle.drawString("Global Currency Intelligence Terminal", 60, 70);
	} else {
		ofDrawBitmapString("Global Currency Intelligence Terminal", 60, 60);
	}

	ofSetColor(120, 135, 160);
	if (hasCustomFont) {
		fontBody.drawString("Retrieve real-time FX rates and calculate accurate target valuations via FreeCurrencyAPI.", 60, 100);
	} else {
		ofDrawBitmapString("Retrieve real-time FX rates and calculate accurate target valuations via FreeCurrencyAPI.", 60, 85);
	}

	ofSetColor(170, 185, 205);
	if (hasCustomFont) {
		fontBody.drawString("Amount:", 60, 148);
		fontBody.drawString("Base Currency:", 370, 148);
		fontBody.drawString("Target Currency:", 610, 148);
	} else {
		ofDrawBitmapString("Amount:", 60, 148);
		ofDrawBitmapString("Base Currency:", 370, 148);
		ofDrawBitmapString("Target Currency:", 610, 148);
	}

	ofSetColor(activeField == ActiveInput::AMOUNT ? ofColor(45, 60, 85) : ofColor(30, 36, 50));
	ofDrawRectRounded(amountBox, 8);
	ofSetColor(activeField == ActiveInput::AMOUNT ? ofColor(64, 153, 255) : ofColor(70, 80, 105));
	ofNoFill();
	ofSetLineWidth(2);
	ofDrawRectRounded(amountBox, 8);
	ofFill();

	ofSetColor(30, 36, 50);
	ofDrawRectRounded(420, 160, 110, 50, 8);
	ofDrawRectRounded(660, 160, 110, 50, 8);

	ofSetColor(45, 55, 75);
	ofDrawRectRounded(basePrevBtn, 8);
	ofDrawRectRounded(baseNextBtn, 8);
	ofDrawRectRounded(targetPrevBtn, 8);
	ofDrawRectRounded(targetNextBtn, 8);

	ofSetColor(isLoading ? ofColor(80, 85, 95) : ofColor(0, 122, 255));
	ofDrawRectRounded(convertBtn, 8);

	ofSetColor(255);
	std::string displayAmount = amountText + (activeField == ActiveInput::AMOUNT && (ofGetFrameNum() % 60 < 30) ? "|" : "");
	if (hasCustomFont) {
		fontHeader.drawString(displayAmount, amountBox.x + 14, amountBox.y + 32);
		fontHeader.drawString("<", basePrevBtn.x + 16, basePrevBtn.y + 32);
		fontHeader.drawString(">", baseNextBtn.x + 16, baseNextBtn.y + 32);
		fontHeader.drawString(currencyList[baseCurrencyIndex], 452, 192);

		fontHeader.drawString("<", targetPrevBtn.x + 16, targetPrevBtn.y + 32);
		fontHeader.drawString(">", targetNextBtn.x + 16, targetNextBtn.y + 32);
		fontHeader.drawString(currencyList[targetCurrencyIndex], 692, 192);

		fontHeader.drawString(isLoading ? "Converting..." : "Convert Now", convertBtn.x + 50, convertBtn.y + 32);
	} else {
		ofDrawBitmapString(displayAmount, amountBox.x + 14, amountBox.y + 30);
		ofDrawBitmapString("<", basePrevBtn.x + 18, basePrevBtn.y + 30);
		ofDrawBitmapString(">", baseNextBtn.x + 18, baseNextBtn.y + 30);
		ofDrawBitmapString(currencyList[baseCurrencyIndex], 460, 190);

		ofDrawBitmapString("<", targetPrevBtn.x + 18, targetPrevBtn.y + 30);
		ofDrawBitmapString(">", targetNextBtn.x + 18, targetNextBtn.y + 30);
		ofDrawBitmapString(currencyList[targetCurrencyIndex], 700, 190);

		ofDrawBitmapString(isLoading ? "Converting..." : "Convert Now", convertBtn.x + 55, convertBtn.y + 30);
	}

	ofSetColor(32, 40, 58);
	ofDrawRectRounded(60, 240, 1010, 55, 8);

	ofSetColor(230, 240, 255);
	if (hasCustomFont) {
		fontBody.drawString("Status: " + statusMessage, 80, 274);
	} else {
		ofDrawBitmapString("Status: " + statusMessage, 80, 274);
	}

	ofSetColor(26, 32, 46);
	ofDrawRectRounded(60, 320, 1010, 220, 10);
	ofSetColor(45, 55, 75);
	ofNoFill();
	ofDrawRectRounded(60, 320, 1010, 220, 10);
	ofFill();

	ofSetColor(140, 155, 180);
	if (hasCustomFont) {
		fontBody.drawString("CONVERTED VALUATION", 100, 370);
	} else {
		ofDrawBitmapString("CONVERTED VALUATION", 100, 370);
	}

	ofSetColor(75, 215, 130);
	if (hasCustomFont) {
		fontLarge.drawString(convertedResult, 100, 435);
	} else {
		ofDrawBitmapString(convertedResult, 100, 430);
	}

	if (!exchangeRateInfo.empty()) {
		ofSetColor(90, 200, 250);
		if (hasCustomFont) {
			fontHeader.drawString("Market Exchange Rate: " + exchangeRateInfo, 100, 495);
		} else {
			ofDrawBitmapString("Market Exchange Rate: " + exchangeRateInfo, 100, 490);
		}
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	if (amountBox.inside(x, y)) {
		activeField = ActiveInput::AMOUNT;
	} else {
		activeField = ActiveInput::NONE;
	}

	if (basePrevBtn.inside(x, y)) {
		baseCurrencyIndex = (baseCurrencyIndex - 1 + currencyList.size()) % currencyList.size();
	} else if (baseNextBtn.inside(x, y)) {
		baseCurrencyIndex = (baseCurrencyIndex + 1) % currencyList.size();
	} else if (targetPrevBtn.inside(x, y)) {
		targetCurrencyIndex = (targetCurrencyIndex - 1 + currencyList.size()) % currencyList.size();
	} else if (targetNextBtn.inside(x, y)) {
		targetCurrencyIndex = (targetCurrencyIndex + 1) % currencyList.size();
	}

	if (convertBtn.inside(x, y) && !isLoading) {
		triggerConversion();
	}
}

void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RETURN) {
		triggerConversion();
		return;
	}

	if (activeField == ActiveInput::AMOUNT) {
		if (key == OF_KEY_BACKSPACE) {
			if (!amountText.empty()) {
				amountText.pop_back();
			}
		} else if ((key >= '0' && key <= '9') || key == '.') {
			if (key == '.' && amountText.find('.') != std::string::npos) {
				return;
			}
			amountText.push_back(static_cast<char>(key));
		}
	}
}
