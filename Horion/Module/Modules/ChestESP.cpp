#include "ChestESP.h"

ChestESP::ChestESP() : IModule('H', Category::VISUAL, "ESP for chests") {
}

ChestESP::~ChestESP() {
}

const char* ChestESP::getModuleName() {
	return ("ChestESP");
}

void ChestESP::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!g_Data.isInGame() || !GameData::canUseMoveKeys() || g_Data.getLocalPlayer() == nullptr)
		return;
	//if (listSize < 1000 && listSize > 1) {
	static float rcolors[4];
	if (rcolors[3] < 1) {
		rcolors[0] = 0.2f;
		rcolors[1] = 0.2f;
		rcolors[2] = 1.f;
		rcolors[3] = 1;
	}
	Utils::ApplyRainbow(rcolors, 0.0015f);

	auto ourListLock = std::scoped_lock(this->listLock);

	for (auto iter = bufferedChestList.begin(); iter != bufferedChestList.end(); ++iter) {
		DrawUtils::setColor(1.f, 0.3f, 0.3f, 0.6f);
		DrawUtils::drawBox((*iter)->lower, (*iter)->upper, max(0.2f, 1 / max(1, g_Data.getLocalPlayer()->eyePos0.dist((*iter)->lower))), true);  // Fancy math to give an illusion of good esp
	}
}

void ChestESP::onTick(C_GameMode* gm) {
	/*tickTimeout++;
	if (tickTimeout > 60) {
		tickTimeout = 0;
		g_Data.clearChestList();
	}*/
	// Swap list
	auto listLock = g_Data.lockChestList();
	auto* chestList = g_Data.getChestList();
	auto ourListLock = std::scoped_lock(this->listLock);

	this->bufferedChestList.clear();
	this->bufferedChestList.insert(this->bufferedChestList.end(), chestList->begin(), chestList->end());
	chestList->clear();
}
