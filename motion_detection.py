import cv2
import numpy
import arena as a
# Defining a function motionDetection
def initialise(cap):
    ret, img = cap.read()
    points= a.get_points(img)
    return points

def motionDetection(cap,points):
    # capturing video in real time
    

    # reading frames sequentially
    ret, frame1 = cap.read()
    frame1 = a.get_arena(points,frame1)
    ret, frame2 = cap.read()
    frame2 = a.get_arena(points,frame2)
    while cap.isOpened():

        # difference between the frames
        diff = cv2.absdiff(frame1, frame2)
        diff_gray = cv2.cvtColor(diff, cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(diff_gray, (5, 5), 0)
        _, thresh = cv2.threshold(blur, 20, 255, cv2.THRESH_BINARY)
        dilated = cv2.dilate(thresh, None, iterations=3)
        contours, _ = cv2.findContours(
            dilated, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        for contour in contours:
            (x, y, w, h) = cv2.boundingRect(contour)
            if cv2.contourArea(contour) < 900:
                continue
            cv2.rectangle(frame1, (x, y), (x+w, y+h), (0, 255, 0), 2)
            cv2.putText(frame1, ".", (x+(w//2), y+(h//2)), cv2.FONT_HERSHEY_SIMPLEX,
                        1, (217, 10, 10), 2)

        # cv.drawContours(frame1, contours, -1, (0, 255, 0), 2)

        cv2.imshow("Video", frame1)
        frame1 = frame2
        ret, frame2 = cap.read()
        frame2 = a.get_arena(points,frame2)

        q = cv2.waitKey(1)
        if q == ord("q"):
                return