#pragma once

#include <string>

namespace Consts
{
	const std::string paths[]{ // 이미지 파일 절대 경로
		// used
		R"(..\threeMatchImage\PNG\ico\1.png)",
		R"(..\threeMatchImage\PNG\ico\2.png)",
		R"(..\threeMatchImage\PNG\ico\13.png)",
		R"(..\threeMatchImage\PNG\ico\14.png)",
		R"(..\threeMatchImage\PNG\ico\16.png)",
		R"(..\threeMatchImage\PNG\ico\17.png)",
		R"(..\threeMatchImage\PNG\ico\20.png)",
		// unused
		R"(..\threeMatchImage\PNG\ico\3.png)",
		R"(..\threeMatchImage\PNG\ico\4.png)",
		R"(..\threeMatchImage\PNG\ico\5.png)",
		R"(..\threeMatchImage\PNG\ico\6.png)",
		R"(..\threeMatchImage\PNG\ico\7.png)",
		R"(..\threeMatchImage\PNG\ico\8.png)",
		R"(..\threeMatchImage\PNG\ico\9.png)",
		R"(..\threeMatchImage\PNG\ico\10.png)",
		R"(..\threeMatchImage\PNG\ico\11.png)",
		R"(..\threeMatchImage\PNG\ico\12.png)",
		R"(..\threeMatchImage\PNG\ico\15.png)",
		R"(..\threeMatchImage\PNG\ico\18.png)",
		R"(..\threeMatchImage\PNG\ico\19.png)",
		R"(..\threeMatchImage\PNG\ico\21.png)",
	};
	const int BOARD_LENGTH = 12;
	const int BOARD_ITEM_SIZE = 60;
	const int MAX_ITEM_TYPES = 6;
	const int ANIMATION_TIME = 200;
}