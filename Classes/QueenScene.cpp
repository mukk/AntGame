//
//  QueenScene.cpp
//  AntGame
//
//  Created by 長谷川 脩人 on 2015/06/17.
//
//

#include "QueenScene.h"
#include "HelloWorldScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

QueenScene::QueenScene()
: _second()
,_count(0)
,_ants()
,_deathCount(0)
,_foods(10)
,_eggs(0)
,_eCount(0)
,_secondLabel(NULL)
,_antsLabel(NULL)
,_foodsLabel(NULL)
,_eggsLabel(NULL)
{
    
}

QueenScene::~QueenScene()
{
    CC_SAFE_RELEASE_NULL(_secondLabel);
    CC_SAFE_RELEASE_NULL(_antsLabel);
    CC_SAFE_RELEASE_NULL(_foodsLabel);
    CC_SAFE_RELEASE_NULL(_eggsLabel);
}

Scene* QueenScene::createScene()
{
    auto scene = Scene::create();
    auto layer = QueenScene::create();
    scene->addChild(layer);
    return scene;
};

void QueenScene::update(float dt)
{
    _count += 1;
    if (_count > 60) {
        
        //秒の増加
        _second += 1;
        int second = static_cast<int>(_second);
        _secondLabel->setString(StringUtils::toString(second));
        
        //時間をユーザーに格納
        UserDefault::getInstance()->setIntegerForKey("secondkey", second);
        
        //食糧の増減
        /*
        if (_second % 3 == 0) {
            _foods += 3 * _ants;
            int foods = static_cast<int>(_foods);
            _foodsLabel->setString(StringUtils::toString(foods));
            UserDefault::getInstance()->setIntegerForKey("foodskey", foods);
        }
        */
        
        if((_second+1) % 3 == 0){
            _foods -= _ants + 1;
            int foods = static_cast<int>(_foods);
            _foodsLabel->setString(StringUtils::toString(foods));
            UserDefault::getInstance()->setIntegerForKey("foodskey", foods);
        }
        
        //卵の孵化
        if(_eggs == 0){
            _eCount = 0;
        }else{
            _eCount += 1;
        }
        if (_eggs > 0) {
            if (_eCount > 10) {
                int eggs = static_cast<int>(_eggs);
                eggs -= 1;
                _eggs = eggs;
                _eggsLabel->setString(StringUtils::toString(eggs));
                UserDefault::getInstance()->setIntegerForKey("eggskey", eggs);
                
                int ants = static_cast<int>(_ants);
                ants += 1;
                _ants = ants;
                _antsLabel->setString(StringUtils::toString(ants));
                UserDefault::getInstance()->setIntegerForKey("antskey", ants);
                
                _eCount = 0;
            }
        }
        
        //アリの死
        if (_ants > 0) {
            _deathCount += 1;
            if (_deathCount > 15) {
                int ants = static_cast<int>(_ants);
                ants -= 1;
                _ants = ants;
                _antsLabel->setString(StringUtils::toString(ants));
                UserDefault::getInstance()->setIntegerForKey("antskey", ants);
                _deathCount = 0;
            }
        }
        
        _count = 0;
    }

}

bool QueenScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM_UnderGround.m4a");
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
    SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM_UnderGround.m4a",true);
    
    auto director = Director::getInstance();//ディレクター
    auto size = director->getWinSize();
    
    //ユーザーから情報を取得
    auto userDefault = UserDefault::getInstance();
    _second = userDefault->getIntegerForKey("secondkey");
    _foods = userDefault->getIntegerForKey("foodskey");
    _eggs = userDefault->getIntegerForKey("eggskey");
    _ants = userDefault->getIntegerForKey("antskey");
    
    //背景
    auto background = Sprite::create("img/roombg.png");
    background->setPosition(Vec2(size.width/2,size.height/2));
    this->addChild(background);
    
    //ラベル
    //秒数
    this->scheduleUpdate();
    int second = static_cast<int>(_second);
    auto secondLabel = Label::createWithSystemFont(StringUtils::toString(second), "Arial", 16);
    this->setSecondLabel(secondLabel);
    secondLabel->setPosition(Vec2(size.width/2 - 200, size.height - 30));
    this->addChild(secondLabel);
    //食糧
    int foods = static_cast<int>(_foods);
    auto foodsLabel = Label::createWithSystemFont(StringUtils::toString(foods), "Arial", 16);
    this->setFoodsLabel(foodsLabel);
    foodsLabel->setPosition(Vec2(size.width/2 - 160, size.height - 30));
    this->addChild(foodsLabel);
    //卵
    int eggs = static_cast<int>(_eggs);
    auto eggsLabel = Label::createWithSystemFont(StringUtils::toString(eggs), "Arial", 16);
    this->setEggsLabel(eggsLabel);
    eggsLabel->setPosition(Vec2(size.width/2 - 120, size.height - 30));
    this->addChild(eggsLabel);
    //蟻
    int ants = static_cast<int>(_ants);
    auto antLabel = Label::createWithSystemFont(StringUtils::toString(ants), "Arial", 16);
    this->setAntsLabel(antLabel);
    antLabel->setPosition(Vec2(size.width/2 - 80, size.height - 30));
    this->addChild(antLabel);
    
    //タッチ
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event){
        return true;
    };
    listener->onTouchMoved = [](Touch* touch, Event* event){
        
    };
    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    //ボタン
    auto button = MenuItemImage::create("img/Lroom.png","img/Lroom.png",[](Ref* ref){
        //シーン移動
        Director::getInstance()->replaceScene(MainScene::createScene());
    });
    auto menu = Menu::create(button,NULL);
    menu->setPosition(Vec2(size.width - 50, 50));
    this->addChild(menu);
    
    //産むボタン
    auto bearButton = MenuItemImage::create("img/btEgg.png","img/Egg.png",[&](Ref* ref){
        int eggs = static_cast<int>(_eggs);
        int foods = static_cast<int>(_foods);
        if (_foods > 10) {//10食糧を消費して、1卵を増やす。
            eggs += 1;
            _eggs = eggs;
            _eggsLabel->setString(StringUtils::toString(eggs));
            UserDefault::getInstance()->setIntegerForKey("eggskey", eggs);
            foods -= 10;
            _foods = foods;
            _foodsLabel->setString(StringUtils::toString(foods));
            UserDefault::getInstance()->setIntegerForKey("foodkey", foods);
        }
        
    });
    auto Qmenu = Menu::create(bearButton,NULL);
    Qmenu->setPosition(Vec2(50,50));
    this->addChild(Qmenu);
    
    return true;
}