#pragma once
#include "ofMain.h"
#define STRINGIFY(A) #A

class ofxShaderTrail {
    public:


//--------------------------------------------------------------
    void setup(float w, float h, float amount=0.044){
        width = w;
        height = h;
        this->amount = amount;

        // Setup two FBOs for ping-ponging
        fbo[0].allocate(width, height, GL_RGB32F);
        fbo[1].allocate(width, height, GL_RGB32F);

        plane.set(width, height, 2, 2);
        plane.mapTexCoords(0, 0, width, height);
        plane.setPosition(width/2, height/2, 0);

        // Setup shader
        loadShader();
    }
//--------------------------------------------------------------
    void update(ofTexture &source){
        fbo[1-currentFbo].begin();
        ofClear(0,0,0);
        ofSetColor(255);
        shader.begin();
        shader.setUniformTexture("video", source, 0);
        shader.setUniformTexture("backbuffer", fbo[currentFbo].getTexture(), 1);
        shader.setUniform1f("amount", amount);
        shader.setUniform2f("resolution", width, height);
        plane.draw();
        shader.end();
        fbo[1-currentFbo].end();
        currentFbo = 1-currentFbo;
    }

//--------------------------------------------------------------    
    void update(ofImage &img){
        update(img.getTexture());
    }

//--------------------------------------------------------------
    void update(ofPixels &pix){
        ofImage img;
        img.setFromPixels(pix);
        update(img.getTexture());
    }
//--------------------------------------------------------------
    void loadShader(){
        string frag;
        if(ofIsGLProgrammableRenderer()){
            ofLogNotice("ofxShaderTrail") << "Using Programmable renderer";

            frag = STRINGIFY(
                out vec4 outColor;
                uniform vec2 resolution;
                uniform SAMPLER_TYPE video;
                uniform SAMPLER_TYPE backbuffer;
                uniform float amount;

                void main(){
                    vec4 color=vec4(0);
                    vec4 back = texture(backbuffer, TEXCOORD);
                    vec4 vid = texture(video, TEXCOORD);
                    color = mix(back,vid,amount);
                    color += back*0.0001;
                    outColor = color;
                }
            );
            string prefix = "#version 150\n";
            if(ofGetUsingArbTex()){
                ofLogNotice("ofxShaderTrail") << "ARB Texture Enabled";
                prefix += "#define SAMPLER_TYPE sampler2DRect\n";
                prefix += "#define TEXCOORD gl_FragCoord.xy\n";
            }else{
                ofLogNotice("ofxShaderTrail") << "ARB Texture Disabled";
                prefix += "#define SAMPLER_TYPE sampler2D\n";
                prefix += "#define TEXCOORD gl_FragCoord.xy/resolution\n";
            }
            frag = prefix + frag;
        }
        else{
            ofLogNotice("ofxShaderTrail") << "Using non programmable renderer";
            frag = STRINGIFY(
                uniform vec2 resolution;
                uniform SAMPLER_TYPE video;
                uniform SAMPLER_TYPE backbuffer;
                uniform float amount;

                void main(){
                    vec4 color=vec4(0);
                    vec4 back = TEXTURE_TYPE(backbuffer, TEXCOORD);
                    vec4 vid = TEXTURE_TYPE(video, TEXCOORD);

                    color = mix(back,vid,amount);
                    color += back*0.0001;
                    gl_FragColor = color;
                }
            );

            string prefix = "#version 120\n";
            if(ofGetUsingArbTex()){
                ofLogNotice("ofxShaderTrail") << "ARB Texture Enabled";
                prefix += "#define SAMPLER_TYPE sampler2DRect\n";
                prefix += "#define TEXCOORD gl_FragCoord.xy\n";
                prefix += "#define TEXTURE_TYPE texture2DRect\n";
            }else{
                ofLogNotice("ofxShaderTrail") << "ARB Texture Disabled";
                prefix += "#define SAMPLER_TYPE sampler2D\n";
                prefix += "#define TEXCOORD gl_FragCoord.xy/resolution\n";
                prefix += "#define TEXTURE_TYPE texture2D\n";
            }
            frag = prefix + frag;
        }

        this->shader.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);

        this->shader.linkProgram();
    }

//--------------------------------------------------------------
    void draw(){
        ofSetColor(255);
        fbo[currentFbo].draw(0,0);
    }
//--------------------------------------------------------------
    void setAmount(float amount){
        this->amount = amount;
    }
//--------------------------------------------------------------
    ofTexture& getTexture(){
        return fbo[currentFbo].getTexture();
    }
    
//--------------------------------------------------------------
    private:
    ofFbo fbo[2];
    ofShader shader;
    unsigned currentFbo;
    float amount;
    float width, height;
    ofPlanePrimitive plane;

};

