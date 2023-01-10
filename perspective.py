# importing the module
import cv2
   
# function to display the coordinates of
# of the points clicked on the image 

points = list()

def clear_points(points_list):
    points_list = []
    return points_list

def get_points():
    return points[:4]


def click_event(event, x, y, flags, params):
  
    # checking for left mouse clicks
    if event == cv2.EVENT_LBUTTONDOWN:
  
        # displaying the coordinates
        # on the Shell
        points.append([x,y])
  
       
        
  
    # checking for right mouse clicks     
    if event==cv2.EVENT_RBUTTONDOWN:
  
        # displaying the coordinates
        # on the Shell
        points.append([x,y])
  
       
  
# driver function
def outline(img):
  
    # reading the image
    
  
    # displaying the image
    cv2.imshow('image', img)
  
    # setting mouse hadler for the image
    # and calling the click_event() function
    cv2.setMouseCallback('image', click_event)
  
    # wait for a key to be pressed to exit
    cv2.waitKey(0)
  
    # close the window
    cv2.destroyAllWindows()
