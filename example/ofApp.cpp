#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);

    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);
    fbo.begin();
    ofClear(0);
    fbo.end();

    trail.setup(ofGetWidth(), ofGetHeight(), trailAmount);
}

//--------------------------------------------------------------
void ofApp::update(){
    // draw something to fbo
    fbo.begin();
    ofClear(0);
    ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    float x = sin(ofGetElapsedTimef()) * 300;
    float y = cos(ofGetElapsedTimef()) * 300;
    ofDrawCircle(x, y, 20);
    ofPopMatrix();

    fbo.end();


    // feed the drawing to the trail
    trail.update(fbo.getTexture()); // accepts ofTexture, ofImage and ofPixels
}

//--------------------------------------------------------------
void ofApp::draw(){
    trail.draw();
    // fbo.draw(0,0);

    // use trail.getTexture() if you have more pipeline afterwards

    stringstream ss;
    ss << "fps: " << ofGetFrameRate() << '\n'
    << "amount: " << trailAmount << '\n'
    << "press +/- to change amount by 0.001";
    ofDrawBitmapString(ss.str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key =='+'){
        trailAmount+=0.001;;
        trail.setAmount(trailAmount);
    }
    if(key =='-'){
        trailAmount-=0.001;;
        trail.setAmount(trailAmount);
    }
}

//--------------------------------------------------------------
