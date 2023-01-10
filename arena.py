import cv2
import numpy as np
import perspective as p


def get_points(img):
    points = []
    p.clear_points(points)
    p.outline(img)
    points = p.get_points()
    return points


def get_arena(points, img):
    width, height = 1000, 500
    pts1 = np.float32(points)
    pts2 = np.float32([[0, 0], [width, 0], [0, height], [width, height]])
    matrix = cv2.getPerspectiveTransform(pts1, pts2)
    imgoutput = cv2.warpPerspective(img, matrix, (width, height))
    return imgoutput


def display(imgoutput):
    cv2.imshow("output", imgoutput)
    cv2.waitKey(0)