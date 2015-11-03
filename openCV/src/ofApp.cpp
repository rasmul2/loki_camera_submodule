#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 640;
	camHeight = 480;

	vidGrabber.setVerbose(this);
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.initGrabber(camWidth, camHeight);


	vidPixels.allocate(camWidth, camHeight, OF_PIXELS_RGB);
	vidTexture.allocate(vidPixels);
	vidImage.allocate(camHeight,camHeight, OF_IMAGE_COLOR); 

	image.allocate(camWidth, camHeight); 
	diffFloat.allocate(camWidth, camHeight); 
	grayImage.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	diff.allocate(camWidth, camHeight);
	bufferFloat.allocate(camWidth, camHeight);
	totalImage.allocate(camWidth, camHeight);
	puppet.allocate(camWidth, camHeight);
	
	sphere.setRadius(10);
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	vidGrabber.update();

	
	
	
		//video.update();		//Decode the new frame if needed
	//Do computing only if the new frame was obtained
	if ( vidGrabber.isFrameNew() ) {
		//Store the previous frame, if it exists till now
		if ( grayImage.bAllocated ) {
			grayImagePrev = grayImage;
		}

		//Getting a new frame
		image.setFromPixels( vidGrabber.getPixelsRef() );
		grayImage = image;	//Convert to grayscale image

		//Do processing if grayImagePrev is inited
		if ( grayImagePrev.bAllocated ) {
			//Get absolute difference
			diff.absDiff( grayImage, grayImagePrev );

			totalImage= grayImage;
		if(totalImage.bAllocated){
			int w = vidTexture.getWidth();
			int h = vidTexture.getHeight();
	
			maxBrightness  = 0; // these are used in the search
			maxBrightnessX = 0; // for the brightest location
			maxBrightnessY = 0;

			// Search through every pixel. If it is brighter than any
			// we've seen before, store its brightness and coordinates.
			// After testing every pixel, we'll know which is brightest!
			for (int y=0; y<h; y++) {
				for(int x=0; x<w; x++) {
					ofColor colorAtXY = vidGrabber.getPixelsRef().getColor(x, y);
					float brightnessOfColorAtXY = colorAtXY.getBrightness();
					if(brightnessOfColorAtXY >= maxBrightness-100.0){
						//totalImage = diff;
						totalImage.getPixelsRef().setColor(x, y, ofColor::white);
						if (brightnessOfColorAtXY > maxBrightness){
							maxBrightness = brightnessOfColorAtXY;
							maxBrightnessX = x;
							maxBrightnessY = y;
						}
					}
					else{
						//vidGrabber.getPixelsRef().setColor(x, y, ofColor:: black);
					}
			
				}
			}
		}

			//We want to amplify the difference to obtain
			//better visibility of motion
			//We do it by multiplication. But to do it, we
			//need to convert diff to float image first
			diffFloat = diff;	//Convert to float image
			diffFloat *= 5.0;	//Amplify the pixel values
			diffFloat *= totalImage;

			//Update the accumulation buffer
			if ( !bufferFloat.bAllocated ) {
				//If the buffer is not initialized, then
				//just set it equal to diffFloat
				bufferFloat = diffFloat;
				//bufferFloat *=totalImage;
			}
			else {
				//Slow damping the buffer to zero
				bufferFloat *= 0.75;
				//Add current difference image to the buffer
				bufferFloat += diffFloat;
				//diffFloat += totalImage;
			}
		}

		
	}

	//float *pixels = bufferFloat.getPixelsAsFloats();
	//Scan all pixels
	//vector for finding cloud middle
	vector <int> xvector;
	vector <int> yvector;

	puppet = bufferFloat;
	int w = vidTexture.getWidth();
	int h = vidTexture.getHeight();
	float *pixels = puppet.getPixelsAsFloats();
	for (int y=0; y< h; y++) {
		for (int x=0; x< w; x++) {
			//Get the pixel value
			//If value exceed threshold, then draw pixel
			float value = pixels[ x + w * y ];
			if ( value >= 1.0 ) {
				puppet.getPixelsRef().setColor(x, y, ofColor::black);
				xvector.push_back(x);
				yvector.push_back(y);
			}
			else{
				puppet.getPixelsRef().setColor(x,y, ofColor::white);
			}
		}
	}

	int xaverage = 0;
	int yaverage = 0;
	for (int i = 0; i < xvector.size(); i++){
		xaverage +=xvector[i];
	}

	for(int j = 0; j <yvector.size(); j++){
		yaverage += yvector[j];
	}
	
	if(xvector.size()> 0){
		xaverage = xaverage/xvector.size();
		yaverage = yaverage/yvector.size();
	}
	

	sphere.setPosition(xaverage, yaverage, 0.0);
	//if(xvector.size()/500 > 10 && xvector.size()/500 < 50){
	//	sphere.setRadius(xvector.size()/500);
	//}
	//if(xvector.size()/500 < 10){
	//	sphere.setRadius(10);
	//}
	//if(xvector.size()/500 > 50){
	//	sphere.setRadius(50);
	//}
}


//--------------------------------------------------------------
void ofApp::draw(){
	//vidTexture.draw(670, 20);
	//vidImage.draw(670, 20);
	////screen.draw(400, 100);



	

	//ofBackground( 255, 255, 255 );	//Set the background color

	//Draw only if diffFloat image is ready.
	//It happens when the second frame from the video is obtained
	if ( diffFloat.bAllocated ) {
		//Get image dimensions
		int w = grayImage.width;	
		int h = grayImage.height;

		//Set color for images drawing
		ofSetColor( 255, 255, 255 );

		//Draw images grayImage,  diffFloat, bufferFloat
		grayImage.draw( 0, 0, w/2, h/2 );
		diffFloat.draw( w/2 + 10, 0, w/2, h/2 );
		bufferFloat.draw( 0, h/2+ 10, w/2, h/2 );
		totalImage.draw(0, h+20, w/2, h/2);
		puppet.draw(w/2 +10, h/2+10, w/2, h/2);
		sphere.setPosition(w/2+10+sphere.getPosition().x/2, h/2+10+sphere.getPosition().y/2,0.0); 

		//Draw the image motion areas

		//Shift and scale the coordinate system
	
	//vidGrabber.draw(20, 20);
	
	}
	ofSetColor(ofColor::blueViolet);
	sphere.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//for (int i = 0; i < camHeight; i++)
//	{
//		vidPixels.setColor(column, i, pixelRef.getColor(160, i));
//	}