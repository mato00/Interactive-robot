import cv2

def CatchChessboard(window_name, camera_idx, catch_pic_num, path_name):
    cv2.namedWindow(window_name)
    cap = cv2.VideoCapture(camera_idx)
    num = 0
    while cap.isOpened():
        ok, frame = cap.read()
        if not ok:
            break

        for i in range(0, 20):
            img_name = '%s/%d.jpg'%(path_name, num)
            cv2.imwrite(img_name, frame)

            num += 1

            font = cv2.FONT_HERSHEY_SIMPLEX
            cv2.putText(frame, 'num:%d'%(num), (30, 30), font, 1, (255,0,255), 4)
            cv2.imshow(window_name, frame)

        if num >= (catch_pic_num):
            break

        c = cv2.waitKey(50)
        if c & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyWindow(window_name)

if __name__ = '__main__':
    CatchChessboard('calibration', 0 ,20, './chessboard')
