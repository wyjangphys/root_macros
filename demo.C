{
  int total = 1000;
  for (int i = 1; i <= total; ++i) {
    // 작업 수행
    gSystem->Sleep(10);  // 예시: 1ms 대기

    // 진행률 출력
    progress_bar(i, total);
  }
}

