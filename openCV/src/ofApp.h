#pragma once


#include "ofMain.h"
#include "ofxOpenCv.h" 




class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofVideoGrabber vidGrabber;
		int camWidth, camHeight;
		ofTexture vidTexture;
		ofPixels vidPixels;
		ofImage vidImage;

		int column;
		int row;
		float maxBrightnessX;
		float maxBrightnessY;
		float maxBrightness;

		ofSpherePrimitive sphere;

		//ofVideoGrabber video;		//Declare the video player object

		ofxCvColorImage image;		//The current video frame

		//The current and the previous video frames as grayscale images
		ofxCvGrayscaleImage grayImage, grayImagePrev;

		ofxCvGrayscaleImage diff;		//Absolute difference of the frames
		ofxCvFloatImage diffFloat;		//Amplified difference images
		ofxCvFloatImage bufferFloat;	//Buffer image
		ofxCvFloatImage totalImage;		//brightness Image
		ofxCvFloatImage puppet;

};