void paintingBoardInitializer(bool) {
    sketchpad = imread("../paintingBoard.png");
    resize(sketchpad, sketchpad, Size(960, 540));
    createTrackbar("Red", "sketchpad", &R_sliderValue, 255);
    createTrackbar("Green", "sketchpad", &G_sliderValue, 255);
    createTrackbar("Blue", "sketchpad", &B_sliderValue, 255);
    moveWindow("sketchpad", 0, 0);
    imshow("sketchpad", sketchpad);
    return;
}
void paintingBoardInitializer () {
    if (waitKey(10) == 'e') paintingBoardInitializer(true);
    return;
}