#include "Umrechnung_01.h"

RGB TranslateColorValues::CMYtoRGB(const CMY& cmy) {
	RGB rgb;
	rgb.Red = 1 - cmy.Cyan;
	rgb.Green = 1 - cmy.Magenta;
	rgb.Blue = 1 - cmy.Yellow;
	return rgb;
}
HSV TranslateColorValues::CMYtoHSV(const CMY& cmy) {
	RGB rgb;
	rgb.Red = 1.0f - cmy.Cyan;
	rgb.Green = 1.0f - cmy.Magenta;
	rgb.Blue = 1.0f - cmy.Yellow;

	HSV hsv;

	float minVal = std::min({ rgb.Red, rgb.Green, rgb.Blue });
	float maxVal = std::max({ rgb.Red, rgb.Green, rgb.Blue });
	float diff = maxVal - minVal;

	if (diff == 0.0f) {
		hsv.Hue = 0.0f;
	}
	else if (maxVal == rgb.Red) {
		hsv.Hue = 60.0f * ((rgb.Green - rgb.Blue) / diff);
		if (hsv.Hue < 0.0f) {
			hsv.Hue += 360.0f;
		}
	}
	else if (maxVal == rgb.Green) {
		hsv.Hue = 60.0f * ((rgb.Blue - rgb.Red) / diff + 2.0f);
		if (hsv.Hue < 0.0f) {
			hsv.Hue += 360.0f;
		}
	}
	else if (maxVal == rgb.Blue) {
		hsv.Hue = 60.0f * ((rgb.Red - rgb.Green) / diff + 4.0f);
		if (hsv.Hue < 0.0f) {
			hsv.Hue += 360.0f;
		}
	}

	if (hsv.Hue < 0.0f) {
		hsv.Hue += 360.0f;
	}

	if (maxVal == 0.0f) {
		hsv.Saturation = 0.0f;
	}
	else {
		hsv.Saturation = diff / maxVal;
	}

	hsv.Value = maxVal;

	return hsv;
}
RGB TranslateColorValues::HSVtoRGB(const HSV& hsv) {
	RGB rgb;
	if (hsv.Saturation == 0.0f) {
		rgb.Red = hsv.Value;
		rgb.Green = hsv.Value;
		rgb.Blue = hsv.Value;
		return rgb;
	}
	else {
		float detailHue = hsv.Hue / 60;
		int hueSection = static_cast<int> (detailHue);
		float hueDiff = detailHue - hueSection;

		float grayComp = hsv.Value * (1.0f - hsv.Saturation);
		float distLastColor = hsv.Value * (1.0f - (hsv.Saturation * hueDiff));
		float distNextColor = hsv.Value * (1.0f - (hsv.Saturation * (1.0f - hueDiff)));

		switch (hueSection)
		{
		case 0: // Rot -> Gelb
			rgb.Red = hsv.Value; rgb.Green = distNextColor; rgb.Blue = grayComp;
			break;
		case 1: // Gelb -> Grün
			rgb.Red = distLastColor; rgb.Green = hsv.Value; rgb.Blue = grayComp;
			break;
		case 2:	// Grün -> Cyan
			rgb.Red = grayComp; rgb.Green = hsv.Value; rgb.Blue = distNextColor;
			break;
		case 3: // Cyan -> Blau
			rgb.Red = grayComp; rgb.Green = distLastColor; rgb.Blue = hsv.Value;
			break;
		case 4:	// Blau -> Magenta
			rgb.Red = distNextColor; rgb.Green = grayComp; rgb.Blue = hsv.Value;
			break;
		case 5:	// Magenta -> Rot
		default:
			rgb.Red = hsv.Value; rgb.Green = grayComp; rgb.Blue = distLastColor;
			break;
		}
	}
	return rgb;
}
CMY TranslateColorValues::HSVtoCMY(const HSV& hsv) {
	CMY cmy;

	if (hsv.Saturation == 0.0f) {
		cmy.Cyan = hsv.Value;
		cmy.Magenta = hsv.Value;
		cmy.Yellow = hsv.Value;
		return cmy;
	}
	else {
		float detailHue = hsv.Hue / 60;
		int hueSection = static_cast<int> (detailHue);
		float hueDiff = detailHue - hueSection;

		float grayComp = hsv.Value * (1.0f - hsv.Saturation);
		float distLastColor = hsv.Value * (1.0f - (hsv.Saturation * hueDiff));
		float distNextColor = hsv.Value * (1.0f - (hsv.Saturation * (1.0f - hueDiff)));

		switch (hueSection)
		{
		case 0: // Rot -> Gelb
			cmy.Cyan = 1 - hsv.Value; cmy.Magenta = 1 - distNextColor; cmy.Yellow = 1 - grayComp;
			break;
		case 1: // Gelb -> Grün
			cmy.Cyan = 1 - distLastColor; cmy.Magenta = 1 - hsv.Value; cmy.Yellow = 1 - grayComp;
			break;
		case 2:	// Grün -> Cyan
			cmy.Cyan= 1 - grayComp; cmy.Magenta = 1 - hsv.Value; cmy.Yellow = 1 - distNextColor;
			break;
		case 3: // Cyan -> Blau
			cmy.Cyan = 1 - grayComp; cmy.Magenta = 1 - distLastColor; cmy.Yellow = 1 - hsv.Value;
			break;
		case 4:	// Blau -> Magenta
			cmy.Cyan = 1 - distNextColor; cmy.Magenta = 1 - grayComp; cmy.Yellow = 1 - hsv.Value;
			break;
		case 5:	// Magenta -> Rot
		default:
			cmy.Cyan = 1 - hsv.Value; cmy.Magenta = 1 - grayComp; cmy.Yellow = 1 - distLastColor;
			break;
		}
	}
	return cmy;
}


void TranslateColorValues::printCMY(const CMY& cmy) {
	std::cout << "CMY-Color: " << cmy.Cyan << "; " << cmy.Magenta << "; " << cmy.Yellow << std::endl;
}
void TranslateColorValues::printHSV(const HSV& hsv) {
	std::cout << "HSV-Color: " << hsv.Hue << "; " << hsv.Saturation << "; " << hsv.Value << std::endl;
}
void TranslateColorValues::printRGB(const RGB& rgb) {
	std::cout << "RGB-Color: " << rgb.Red << "; " << rgb.Green << "; " << rgb.Blue << std::endl;
}





void TranslateColorValues::handleInputTask01() {
	int choice = 0;
	std::cout << "Welche Konvertierung soll angewendet werden?\n0. skip\n1. CMY -> RGB\n2. CMY -> HSV\n3. HSV -> RGB\n4. HSV -> CMY\n";
	std::cin >> choice;
	if (choice == 0) return;

	std::cout << "Gib nun die 3 Werte ein:\n";
	float v1 = 0.0f;
	float v2 = 0.0f;
	float v3 = 0.0f;

	std::cin >> v1 >> v2 >> v3;

	switch (choice)
	{
	case 1:
	{
		CMY cmy = { v1, v2, v3 };
		printRGB(CMYtoRGB(cmy));
		break;
	}
	case 2:
	{
		CMY cmy = { v1, v2, v3 };
		printHSV(CMYtoHSV(cmy));
		break;
	}
	case 3:
	{
		HSV hsv = { v1, v2, v3 };
		printRGB(HSVtoRGB(hsv));
		break;
	}
	case 4:
	{
		HSV hsv = { v1, v2, v3 };
		printCMY(HSVtoCMY(hsv));
		break;
	}
	default:
		break;
	}
}
RGB TranslateColorValues::handleInputTask02() {
	int choice = 0;
	RGB rgb = {0,0,0};

	std::cout << "Welcher Farbraum soll für das Quadrat genutzt werden?\n0. skip\n1. RGB\n2. CMY\n3. HSV\n";
	std::cin >> choice;
	if (choice == 0) return rgb;

	std::cout << "Gib nun die 3 Werte ein:\n";
	float v1 = 0.0f;
	float v2 = 0.0f;
	float v3 = 0.0f;
	std::cin >> v1 >> v2 >> v3;

	switch (choice)
	{
	case 1:
		rgb.Red = v1;
		rgb.Green = v2;
		rgb.Blue = v3;
		break;
	case 2:
		rgb = CMYtoRGB({ v1, v2, v3 });
		break;
	case 3:
		rgb = HSVtoRGB({ v1, v2, v3 });
		break;
	default:
		break;
	}
	return rgb;
}