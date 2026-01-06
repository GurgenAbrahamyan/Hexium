#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../core/EventBus.h"
#include "../core/Event.h"
#include "../core/EngineContext.h"

class MouseInput {

	bool cameraMode = false;
	EventBus* eventbus;
	GLFWwindow* window = EngineContext::get().getWindow();
public:
	MouseInput(EventBus* eventbus) :
		eventbus(eventbus){}
	
	

	void proccessInput() {

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cameraMode = true;
			CameraMode e(true);
			
			eventbus->publish(e); 
			glfwSetInputMode(EngineContext::get().getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			/*if(!cameraMode){
			
			ClickPos e(true);
			eventbus->publish(e); 
			}
			
			
			*/
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			cameraMode = false;
			CameraMode e(false);

			eventbus->publish(e);
			glfwSetInputMode(EngineContext::get().getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			/*if(!cameraMode){

			ClickPos e(true);
			eventbus->publish(e);
			}


			*/
		}


		if (cameraMode) {

			double xpos, ypos;
			int xmid = EngineContext::get().windowWidth / 2, ymid = EngineContext::get().windowHeight / 2;
			
			glfwGetCursorPos(window, &xpos, &ypos);

			

			double xoffset = xpos - xmid;
			double yoffset = ymid - ypos; 

		


			if (xoffset != 0 || yoffset != 0) {
				MouseDragged m(xoffset, yoffset);
				eventbus->publish(m);
				glfwSetCursorPos(window, xmid, ymid);
			}

		}



	}
};