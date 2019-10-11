int array_judge (int arr[]) {
    int max = -1;
    int index = 0;
    for (int i = 2; i < 5; ++i) {
        if (arr[i] > max){
            index = i;
            max = arr[i];
        }
    }
    //cout << index <<' ';
    return index;
}