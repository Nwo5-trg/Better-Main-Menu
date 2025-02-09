#include "include.hpp"
// btw half way thru this this project i realised i dont have to add cocos2d:: to the start of everything but im just keeping it that way because i dont wanna change it
// remind me to clean up this code next feature update i do because i can make this a lot cleaner
// variables
bool modCompatibility = false;
bool searchFilter = false;
bool searchFilterAlt = false;
auto geodeloader = Loader::get();
// not using arrays because i cant be fucked and this is easier and probably wont be unoptimised hopefully :3 also cuz this mod is inherently incompatible with every ui mod (unless i manually add compatiblity for them) so i can't be fucked to use layouts for button positions
// left tab
int leftTabButtonOffset = 25;
auto leftTabPositionOne = cocos2d::CCPoint(leftTabButtonOffset, 245);
auto leftTabPositionTwo = cocos2d::CCPoint(leftTabButtonOffset, 221.25);
auto leftTabPositionThree = cocos2d::CCPoint(leftTabButtonOffset, 197.5);
auto leftTabPositionFour = cocos2d::CCPoint(leftTabButtonOffset, 173.75);
auto leftTabPositionFive = cocos2d::CCPoint(leftTabButtonOffset, 150);
auto leftTabPositionSix = cocos2d::CCPoint(leftTabButtonOffset, 126.25);
auto leftTabPositionSeven = cocos2d::CCPoint(leftTabButtonOffset, 102.5);
auto leftTabPositionEight = cocos2d::CCPoint(leftTabButtonOffset, 78.75);
auto leftTabPositionNine = cocos2d::CCPoint(leftTabButtonOffset, 50);
// right tab
int rightTabButtonOffset = 570 - 10; // expression literally just there so i dont have to type 560 lol
auto rightTabPositionOne = cocos2d::CCPoint(rightTabButtonOffset, 300);
auto rightTabPositionTwo = cocos2d::CCPoint(rightTabButtonOffset, 276.25);
auto rightTabPositionThree = cocos2d::CCPoint(rightTabButtonOffset, 252.5);
auto rightTabPositionFour = cocos2d::CCPoint(rightTabButtonOffset, 228.75);
auto rightTabPositionFive = cocos2d::CCPoint(rightTabButtonOffset, 205);
auto rightTabPositionSix = cocos2d::CCPoint(rightTabButtonOffset, 181.25);
auto rightTabPositionSeven = cocos2d::CCPoint(rightTabButtonOffset, 157.5);

// search for the thing on init when you use search filter widget
class $modify(LevelSearchLayer) {
    bool init(int p0) {
        if (!LevelSearchLayer::init(p0)) return false;
            if (searchFilter) {
                this->m_searchInput->setString(Mod::get()->getSettingValue<std::string>("search-filter"));
                this->onSearch(nullptr);
                // no reason i need both but im paranoid ok, the voices tell me to
                searchFilter = false;
                searchFilterAlt = false;
            }
            else if (searchFilterAlt) {
                this->m_searchInput->setString(Mod::get()->getSettingValue<std::string>("alt-search-filter"));
                this->onSearch(nullptr);
                searchFilter = false;
                searchFilterAlt = false;
            }
        return true;
    }
};

class $modify(MenuLayerUwU, MenuLayer) {
    // make game not crash when trying to move texture loader button (thank you gdtweaks)
     static void onModify(auto& self)
    {
        auto returnValueIWillNeverUseButNeedForIForgotWhatReason = self.setHookPriority("MenuLayer::init", -99);
    }
    // do i need these to take ccobject as input, i dont think so, am i keeping it anyway, yeah
    void openModSettings(CCObject* sender) {
        if (auto mod = Loader::get()->getLoadedMod(Mod::get()->getSettingValue<std::string>("settings-button-mod"))) {
            geode::openSettingsPopup(mod);
        }
        else {
            FLAlertLayer::create(
                "Invalid Mod ID",
                "what the title says",
                "OK"
            )->show();
        }
    }

    void openModSettingsAlt(CCObject* sender) {
        if (auto mod = Loader::get()->getLoadedMod(Mod::get()->getSettingValue<std::string>("alt-settings-button-mod"))) {
            geode::openSettingsPopup(mod);
        }
        else {
            FLAlertLayer::create(
                "Invalid Mod ID",
                "what the title says",
                "OK"
            )->show();
        }
    }

    void setSearch(CCObject* sender) {
        // idk if this will work on windows, uses out of line thing idk if thatll work or not
        searchFilter = true;
        searchFilterAlt = false;
        auto creatorLayer = CreatorLayer::create();
        creatorLayer->onOnlineLevels(nullptr);
        creatorLayer->removeMeAndCleanup();
    }

    void setSearchAlt(CCObject* sender) {
        searchFilter = false;
        searchFilterAlt = true;
        auto creatorLayer = CreatorLayer::create();
        creatorLayer->onOnlineLevels(nullptr);
        creatorLayer->removeMeAndCleanup();
    }

    void geodeButton (CCObject* sender) {
        if (auto geodeButton = this->getChildByIDRecursive("geode.loader/geode-button")) {
            static_cast<CCMenuItemSpriteExtra*>(geodeButton)->activate();
        }
        // not even adding a warning here bro if this happens we got bigger problems, not like the other warnings should ever happen anyway but still
    }

    void textureLoaderButton (CCObject* sender) {
        if (geodeloader->isModLoaded("geode.texture-loader")) {
            if (auto textureLoaderButton = this->getChildByIDRecursive("geode.texture-loader/texture-loader-button")) {
                static_cast<CCMenuItemSpriteExtra*>(textureLoaderButton)->activate();
            }
            else{
                log::warn("textureloadernotfound");
            } 
        }
    }

    void globedButton (CCObject* sender) {
        if (geodeloader->isModLoaded("dankmeme.globed2")) {
            if (auto globedButton = this->getChildByIDRecursive("dankmeme.globed2/main-menu-button")) {
                static_cast<CCMenuItemSpriteExtra*>(globedButton)->activate();
            }
            else{
                log::warn("globednotfound");
            } 
        }
    }

	bool init() {
		if (!MenuLayer::init()) return false;
    
if (Mod::get()->getSettingValue<bool>("main-menu-enabled")) {
    // int statManaOrbs;
    // statManaOrbs = GameStatsManager::get()->getStat("14");
    // log::error("{}", statManaOrbs);

    // make new menu
    auto mainMenu = CCMenu::create();
    mainMenu->setPosition(cocos2d::CCPoint(0, 0));
    this->addChild(mainMenu);
    mainMenu->setID("better-main-menu");

    // make sidebar menu
    auto sideBarMenu = CCMenu::create();
    sideBarMenu->setPosition(cocos2d::CCPoint(0, 0));
    mainMenu->addChild(sideBarMenu);
    sideBarMenu->setID("better-main-menu-side-menu");
    auto sideBarSpr = CCSprite::create("mainsidebarspr.png"_spr);
    sideBarMenu->addChild(sideBarSpr);
    sideBarSpr->setPosition(cocos2d::CCPoint(500, 160)), sideBarSpr->setScaleX(2.8), sideBarSpr->setScaleY(4);

    // make extra buttons Menu
    auto extraButtonsMenu = CCMenu::create();
    sideBarMenu->addChild(extraButtonsMenu);
    extraButtonsMenu->setID("extra-buttons-menu");
    extraButtonsMenu->setContentSize(cocos2d::CCSize(40, 77.5));
    extraButtonsMenu->setLayout(ColumnLayout::create()->setGap(5)->setAxisAlignment(geode::AxisAlignment::Start)->setAutoScale(false));
    
    auto extraButtonsMenuTwo = CCMenu::create();
    sideBarMenu->addChild(extraButtonsMenuTwo);
    extraButtonsMenuTwo->setID("extra-buttons-menu-two");
    extraButtonsMenuTwo->setContentSize(cocos2d::CCSize(40, 77.5));
    extraButtonsMenuTwo->setLayout(ColumnLayout::create()->setGap(5)->setAxisAlignment(geode::AxisAlignment::Start)->setAutoScale(false));
    
    // shift when the profile button does
    if (!modCompatibility && !Mod::get()->getSettingValue<bool>("shift-profile-button")) {
        extraButtonsMenu->setPosition(cocos2d::CCPoint(449, 50));
        extraButtonsMenuTwo->setPosition(cocos2d::CCPoint(549.5, 50));
    }
    else {
        extraButtonsMenu->setPosition(cocos2d::CCPoint(454, 50));
        extraButtonsMenuTwo->setPosition(cocos2d::CCPoint(484, 50));
    }
    // this is like if i did the menu thing with layouts instead of set positions
    auto setupWidget = [&] (std::string settingValue, bool type) {
        CCMenuItemSpriteExtra* button;

        if (settingValue == "Mod Settings") {
            button = CCMenuItemSpriteExtra::create(CircleButtonSprite::create(CCSprite::create("settingsspr.png"_spr)), this, menu_selector(MenuLayerUwU::openModSettings));
            button->setScale(0.5f), static_cast<CCMenuItemSpriteExtra*>(button)->m_baseScale = 0.5f;
            button->setID("widget-open-mod-settings");
        }
        else if (settingValue == "Alt Mod Settings") {
            button = CCMenuItemSpriteExtra::create(CircleButtonSprite::create(CCSprite::create("altsettingsspr.png"_spr)), this, menu_selector(MenuLayerUwU::openModSettingsAlt));
            button->setScale(0.5f), static_cast<CCMenuItemSpriteExtra*>(button)->m_baseScale = 0.5f;
            button->setID("widget-alt-open-mod-settings");
        }
        else if (settingValue == "Refresh Menu") {
            button = CCMenuItemSpriteExtra::create(CircleButtonSprite::create(CCSprite::create("refreshspr.png"_spr)), this, menu_selector(CreatorLayer::onBack));
            button->setScale(0.5f), static_cast<CCMenuItemSpriteExtra*>(button)->m_baseScale = 0.5f;
            button->setID("widget-refresh-menu");
        }
        else if (settingValue == "Search Filter") {
            button = CCMenuItemSpriteExtra::create(CircleButtonSprite::create(CCSprite::create("searchspr.png"_spr)), this, menu_selector(MenuLayerUwU::setSearch));
            button->setScale(0.5f), static_cast<CCMenuItemSpriteExtra*>(button)->m_baseScale = 0.5f;
            button->setID("widget-search-filter");
        }
        else if (settingValue == "Alt Search Filter") {
            button = CCMenuItemSpriteExtra::create(CircleButtonSprite::create(CCSprite::create("altsearchspr.png"_spr)), this, menu_selector(MenuLayerUwU::setSearchAlt));
            button->setScale(0.5f), static_cast<CCMenuItemSpriteExtra*>(button)->m_baseScale = 0.5f;
            button->setID("widget-alt-search-filter");
        }
        else if (settingValue == "Empty") {
            return;
        }
        if (type) {
            extraButtonsMenu->addChild(button);
            extraButtonsMenu->updateLayout();
        }
        else {
            extraButtonsMenuTwo->addChild(button);
            extraButtonsMenuTwo->updateLayout();
        }
    };
    // setup the widgets
    // menu one
    setupWidget(Mod::get()->getSettingValue<std::string>("widget-button-one"), true);
    // menu two
    setupWidget(Mod::get()->getSettingValue<std::string>("widget-button-two"), true);
    setupWidget(Mod::get()->getSettingValue<std::string>("widget-button-three"), true);
    setupWidget(Mod::get()->getSettingValue<std::string>("widget-button-four"), false);
    setupWidget(Mod::get()->getSettingValue<std::string>("widget-button-five"), false);
    setupWidget(Mod::get()->getSettingValue<std::string>("widget-button-six"), false);
    // remove old menus
    this->removeChild(getChildByID("close-menu"));
    this->removeChild(getChildByID("social-media-menu"));
    this->removeChild(getChildByID("more-games-menu"));
    this->removeChild(getChildByID("main-menu"));    
    
    // move the mod buttons out of the way and stuff
    if (auto geodeMenu = this->getChildByIDRecursive("geode.loader/geode-button")) { 
        // i would move the updates avaliable thing so i could have that but idk how to get that node because it only loads after geode searched the index for updates and that's not in the docs and im too dumb to figure it out myself
        geodeMenu->setVisible(false); 
        mainMenu->addChild(geodeMenu);
    }

    if (geodeloader->isModLoaded("geode.texture-loader")) {
        if (auto textureLoaderButton = this->getChildByIDRecursive("geode.texture-loader/texture-loader-button")) {   
            textureLoaderButton->setVisible(false);
            mainMenu->addChild(textureLoaderButton);
        }
    }

    if (geodeloader->isModLoaded("dankmeme.globed2")) {
        if (auto globedButton = getChildByIDRecursive("dankmeme.globed2/main-menu-button")) {              
            globedButton->setVisible(false);
            mainMenu->addChild(globedButton);
        }
    }
    // delete menus where mod buttons are
    this->removeChild(this->getChildByID("bottom-menu"));
    this->removeChild(this->getChildByID("right-side-menu"));
    // move profile
    // if statement incase a mod gets released that i need to add compatibility for (useless for now)
    if (!modCompatibility && !Mod::get()->getSettingValue<bool>("shift-profile-button")) {
        this->getChildByID("profile-menu")->setPosition(cocos2d::CCPoint(559.5, 50)), this->getChildByID("profile-menu")->setScale(1.25);
        mainMenu->addChild(this->getChildByID("profile-menu"));
        this->getChildByID("player-username")->setPosition(cocos2d::CCPoint(499.5, 100));
        mainMenu->addChild(this->getChildByID("player-username"));
    }
    else {
        // this->getChildByID("profile-menu")->setPosition(cocos2d::CCPoint(585, 50)), this->getChildByID("profile-menu")->setScale(1.25);
        this->getChildByID("profile-menu")->setPosition(cocos2d::CCPoint(581, 40));
        mainMenu->addChild(this->getChildByID("profile-menu"));
        // this->getChildByID("player-username")->setPosition(cocos2d::CCPoint(525, 100));
        this->getChildByID("player-username")->setPosition(cocos2d::CCPoint(532.5, 80));
        mainMenu->addChild(this->getChildByID("player-username"));
    }
    // move title
    this->getChildByID("main-title")->setPosition(cocos2d::CCPoint(170, 290));
    this->getChildByID("main-title")->setScale(0.75);
    // font shit
    int fontNumber = Mod::get()->getSettingValue<int64_t>("menu-font");
    std::string font;
    float fontScale;

    if (fontNumber == 0) {
        font = "bigFont.fnt";
        fontScale = 0.75;
    }
    else if (fontNumber <= 59) {
        font = std::string("gjFont") + (fontNumber < 10 ? "0" : "") + std::to_string(fontNumber) + ".fnt";
        fontScale = 0.75;
    }
    else if (fontNumber == 60) {
        font = "chatFont.fnt";
        fontScale = 1.75;
    }
    else if (fontNumber == 61) {
        font = "goldFont.fnt";
        fontScale = 1;
    }
    // setup button lambda
    auto setupButton = [&] (std::string settingValue, cocos2d::CCPoint const& position, bool type) {
        CCMenuItemSpriteExtra* button;
        CCNode* modCompatibilityButton;

        if (settingValue == "Play") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("play-label").c_str(), font.c_str()), this, menu_selector(MenuLayer::onPlay)); 
        }
        else if (settingValue == "Online") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("online-label").c_str(), font.c_str()), this, menu_selector(MenuLayer::onCreator));
        }
        else if (settingValue == "Icon") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("icon-label").c_str(), font.c_str()), this, menu_selector(MenuLayer::onGarage));
        }
        else if (settingValue == "Settings") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("settings-label").c_str(), font.c_str()), this, menu_selector(MenuLayer::onOptions));
        }
        else if (settingValue == "Stats") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("stats-label").c_str(), font.c_str()), this, menu_selector(MenuLayer::onStats)); 
        }
        else if (settingValue == "Quit") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("quit-label").c_str(), font.c_str()), this, menu_selector(MenuLayer::onQuit));
        }
        else if (settingValue == "Chest") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("chest-label").c_str(), font.c_str()), this, menu_selector(MenuLayer::onDaily));    
        }
        else if (settingValue == "Create") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("create-label").c_str(), font.c_str()), this, menu_selector(CreatorLayer::onMyLevels));
        }
        else if (settingValue == "Search") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("search-label").c_str(), font.c_str()), this, menu_selector(CreatorLayer::onOnlineLevels));
        }
        else if (settingValue == "Daily") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("daily-label").c_str(), font.c_str()), this, menu_selector(CreatorLayer::onDailyLevel));    
        }
        else if (settingValue == "Weekly") {
            button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("weekly-label").c_str(), font.c_str()), this, menu_selector(CreatorLayer::onWeeklyLevel));
        }
        else if (settingValue == "Mods") {
            if (this->getChildByIDRecursive("geode.loader/geode-button")) {
                button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("mods-label").c_str(), font.c_str()), this, menu_selector(MenuLayerUwU::geodeButton));
            }
            else {
                return;
            }
        }
        else if (settingValue == "Textures") {
            if (this->getChildByIDRecursive("geode.texture-loader/texture-loader-button")) {
                button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("textures-label").c_str(), font.c_str()), this, menu_selector(MenuLayerUwU::textureLoaderButton));
            }
            else {
                return;
            }
        }
        else if (settingValue == "Globed") {
            if (this->getChildByIDRecursive("dankmeme.globed2/main-menu-button")) {
                button = CCMenuItemSpriteExtra::create(CCLabelBMFont::create(Mod::get()->getSettingValue<std::string>("globed-label").c_str(), font.c_str()), this, menu_selector(MenuLayerUwU::globedButton));
            }
            else {
                return;
            }
        }
        else if (settingValue == "Empty") {
            return;
        }

        if (type) {
            mainMenu->addChild(button);
            button->setAnchorPoint(cocos2d::CCPoint(0, 0.5));
            button->setID(fmt::format("left-side-button-{}", position.y));
        }
        else {
            sideBarMenu->addChild(button);
            button->setAnchorPoint(cocos2d::CCPoint(1, 0.5));
            button->setID(fmt::format("right-side-button-{}", position.y));
        }

        button->setPosition(position);
        button->m_baseScale = fontScale;
        button->setScale(fontScale);

    };
    // setup the buttons
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-one"), leftTabPositionOne, true);
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-two"), leftTabPositionTwo, true);
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-three"), leftTabPositionThree, true);
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-four"), leftTabPositionFour, true);
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-five"), leftTabPositionFive, true);
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-six"), leftTabPositionSix, true);
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-seven"), leftTabPositionSeven, true);
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-eight"), leftTabPositionEight, true);
    setupButton(Mod::get()->getSettingValue<std::string>("left-tab-slot-nine"), leftTabPositionNine, true);
    setupButton(Mod::get()->getSettingValue<std::string>("right-tab-slot-one"), rightTabPositionOne, false);
    setupButton(Mod::get()->getSettingValue<std::string>("right-tab-slot-two"), rightTabPositionTwo, false);
    setupButton(Mod::get()->getSettingValue<std::string>("right-tab-slot-three"), rightTabPositionThree, false);
    setupButton(Mod::get()->getSettingValue<std::string>("right-tab-slot-four"), rightTabPositionFour, false);
    setupButton(Mod::get()->getSettingValue<std::string>("right-tab-slot-five"), rightTabPositionFive, false);
    setupButton(Mod::get()->getSettingValue<std::string>("right-tab-slot-six"), rightTabPositionSix, false);
    setupButton(Mod::get()->getSettingValue<std::string>("right-tab-slot-seven"), rightTabPositionSeven, false);
}

		return true;    
	}
};