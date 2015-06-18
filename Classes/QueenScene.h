//
//  QueenScene.h
//  AntGame
//
//  Created by 長谷川 脩人 on 2015/06/17.
//
//

#ifndef __AntGame__QueenScene__
#define __AntGame__QueenScene__

#include <stdio.h>
#include "cocos2d.h"

class QueenScene :public cocos2d::Layer
{
protected:
    QueenScene();
    virtual ~QueenScene();
    bool init() override;
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(QueenScene);
    void update(float dt);
    
    CC_SYNTHESIZE(int, _second, Second);//秒
    CC_SYNTHESIZE(int, _count, Count);//0.17秒カウント
    CC_SYNTHESIZE(int, _oldSecond, OldSecond);
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _secondLabel, SecondLabel);//秒ラベル
    
    CC_SYNTHESIZE(int, _ants, Ants);//アリの数
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _antsLabel, AntsLabel);
    CC_SYNTHESIZE(int, _deathCount, DeathCount);//アリの寿命カウント
    
    CC_SYNTHESIZE(int, _foods, Foods);//食糧の数
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _foodsLabel, FoodsLabel);
    
    CC_SYNTHESIZE(int, _eggs, Eggs);//卵の数
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _eggsLabel, EggsLabel);
    CC_SYNTHESIZE(int, _eCount, ECount);//卵の生まれるカウント
    
};


#endif /* defined(__AntGame__QueenScene__) */
