//
//  QueenScene.cpp
//  AntGame
//
//  Created by 長谷川 脩人 on 2015/06/17.
//
//

#include "FoodScene.h"
#include "HelloWorldScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

FoodScene::FoodScene()
: _second()
,_count(0)
,_ants()
,_deathCount(0)
,_foods()
,_eggs()
,_eCount(0)
,_secondLabel(NULL)
,_antsLabel(NULL)
,_foodsLabel(NULL)
,_eggsLabel(NULL)
{
    
}

FoodScene::~FoodScene()
{
    CC_SAFE_RELEASE_NULL(_secondLabel);
    CC_SAFE_RELEASE_NULL(_antsLabel);
    CC_SAFE_RELEASE_NULL(_foodsLabel);
    CC_SAFE_RELEASE_NULL(_eggsLabel);
}

Scene* FoodScene::createScene()
{
    auto scene = Scene::create();
    auto layer = FoodScene::create();
    scene->addChild(layer);
    return scene;
};

void FoodScene::update(float dt)
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
        if (_second % 3 == 0) {//アリの分だけ食糧が増える
            _foods += 3 * _ants;
            int foods = static_cast<int>(_foods);
            _foodsLabel->setString(StringUtils::toString(foods));
            UserDefault::getInstance()->setIntegerForKey("foodskey", foods);
        }
         */
        
        if(_second % 5 == 0){
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
        _fCount += 1;//食糧取得可能カウンター
        _count = 0;
    }
    
}

bool FoodScene::init()
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
    
    //狩りボタン
    auto huntButton = MenuItemImage::create("img/btGetF.png","img/btGetF.png",[&](Ref* ref){
        if (_fCount > 5) {//食糧取得カウンタが満たされていれば取得可能
            int foods = static_cast<int>(_foods);
            foods += 10 * _ants;
            _foods = foods;
            _foodsLabel->setString(StringUtils::toString(foods));
            UserDefault::getInstance()->setIntegerForKey("foodskey", foods);//アリの数だけ食糧を得る
        
            int randNum = arc4random()%(_ants/3);//アリ全体の数/3　-　1の中のランダムな値
            int ants = static_cast<int>(_ants);
            ants -= randNum;
            _ants = ants;
            _antsLabel->setString(StringUtils::toString(ants));
            UserDefault::getInstance()->setIntegerForKey("antskey", ants);//アリがランダムな値の分だけ死ぬ
            _fCount = 5;
        }

    });
    auto Hmenu = Menu::create(huntButton,NULL);
    Hmenu->setPosition(Vec2(50,50));
    this->addChild(Hmenu);

    
    return true;
}