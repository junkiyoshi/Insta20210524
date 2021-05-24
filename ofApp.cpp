#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->line.clear();

	for (int x = -350; x <= 350; x += 8) {

		for (int y = -350; y <= 350; y += 8) {

			auto height = ofMap(ofNoise(x, y, ofGetFrameNum() * 0.005), 0, 1, 0, 200);
			auto rad = std::atan2(y, x);
			auto noise_value = ofNoise(cos(rad), sin(rad), this->noise_param);
			auto radius = ofMap(noise_value, 0, 1, 100, 350);
			auto distance = glm::distance(glm::vec2(radius * cos(rad), radius * sin(rad)), glm::vec2(x, y));

			if (distance < 50) {
			
				height *= ofMap(distance, 0, 50, 1, 0);
				this->setBoxToMesh(this->face, this->line, glm::vec3(x, y, height * -0.5), 8, 8, height, ofColor(239));
			}
			else {

				this->setBoxToMesh(this->face, this->line, glm::vec3(x, y, -4), 8, 8, 8, ofColor(239));
			}
		}
	}

	if (ofGetFrameNum() % 70 < 45) {

		this->noise_param += ofMap(ofGetFrameNum() % 70, 0, 45, 0.07, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(130);

	this->face.draw();

	ofSetColor(39);
	this->line.drawWireframe();

	ofMesh ring, ring_line;
	ring_line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	vector<glm::vec3> vertices;
	for (float deg = 0; deg <= 360; deg += 1) {

		auto noise_value = ofNoise(cos(deg * DEG_TO_RAD), sin(deg * DEG_TO_RAD), this->noise_param);
		auto radius = ofMap(noise_value, 0, 1, 100, 350);
		
		vertices.push_back(glm::vec3((radius - 15) * cos(deg * DEG_TO_RAD), (radius - 15) * sin(deg * DEG_TO_RAD), -300));
		vertices.push_back(glm::vec3((radius + 15) * cos(deg * DEG_TO_RAD), (radius + 15) * sin(deg * DEG_TO_RAD), -300));

		ring.addVertices(vertices);
		ring_line.addVertices(vertices);
		if (deg > 0) {

			ring.addIndex(ring.getNumVertices() - 1); ring.addIndex(ring.getNumVertices() - 2); ring.addIndex(ring.getNumVertices() - 4);
			ring.addIndex(ring.getNumVertices() - 1); ring.addIndex(ring.getNumVertices() - 4); ring.addIndex(ring.getNumVertices() - 5);

			ring_line.addIndex(ring_line.getNumVertices() - 1); ring_line.addIndex(ring_line.getNumVertices() - 3);
			ring_line.addIndex(ring_line.getNumVertices() - 2); ring_line.addIndex(ring_line.getNumVertices() - 4);
		}
	}

	ofSetColor(239);
	ring.draw();
	ofSetColor(39);
	ring_line.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& line_target, glm::vec3 location, float size, ofColor face_color) {

	this->setBoxToMesh(face_target, line_target, location, size, size, size, face_color);
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& line_target, glm::vec3 location, float height, float width, float depth, ofColor face_color) {

	int face_index = face_target.getNumVertices();
	int line_index = line_target.getNumVertices();

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 1);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

	face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
	face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 1);

	face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);
	face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 2);

	face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 4);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 3);

	line_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	line_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	line_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	line_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	line_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	line_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	line_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	line_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	line_target.addIndex(line_index + 0); line_target.addIndex(line_index + 1);
	line_target.addIndex(line_index + 1); line_target.addIndex(line_index + 2);
	line_target.addIndex(line_index + 2); line_target.addIndex(line_index + 3);
	line_target.addIndex(line_index + 3); line_target.addIndex(line_index + 0);

	line_target.addIndex(line_index + 4); line_target.addIndex(line_index + 5);
	line_target.addIndex(line_index + 5); line_target.addIndex(line_index + 6);
	line_target.addIndex(line_index + 6); line_target.addIndex(line_index + 7);
	line_target.addIndex(line_index + 7); line_target.addIndex(line_index + 4);

	line_target.addIndex(line_index + 0); line_target.addIndex(line_index + 4);
	line_target.addIndex(line_index + 1); line_target.addIndex(line_index + 5);
	line_target.addIndex(line_index + 2); line_target.addIndex(line_index + 6);
	line_target.addIndex(line_index + 3); line_target.addIndex(line_index + 7);

	for (int i = 0; i < 8; i++) {

		face_target.addColor(face_color);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}