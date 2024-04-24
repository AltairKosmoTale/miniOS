# miniOS
  
miniOS-project/  
├── README.md					# 프로젝트 설명 및 사용 방법 문서  
├── Makefile					# 전체 프로젝트 빌드 자동화를 위한 메이크파일  
├── boot/						# 부트로더 소스 코드  
O	└── boot.asm				# (yet)부트로더 어셈블리 코드  
├── kernel/						# 커널 소스 코드  
O	├── kernel.c				# 커널 메인 C 소스 파일  
O	├── process.c				# (yet)  
O	├── system.c				# Assigment4: minisystem, smile (Test 함수)  
O	└── 20201590/				# Assigment 모음  
O		├── pi.c					# Assignment7: multi-thread test 코드  
O		├── pipe_test.c				# Assignment6: pipe test 코드  
O		├── shared_memory_test.c	# Assignment6: shared memory test 코드  
O		├── fork_test.c				# Assignment5: fork() test 코드  
O		└── ssu_id.c				# Assignment5: 학번 출력하는 코드  
├── drivers/				# (yet)디바이스 드라이버 코드  
O	├── keyboard.c			# (yet)키보드 드라이버  
O	├── screen.c			# (yet)화면(비디오) 드라이버  
O	└── ...  
├── lib/					# (yet)커널 라이브러리 및 공통 유틸리티  
O	├── stdio.c				# (yet)기본 입출력 함수  
O	├── string.c			# (yet)문자열 처리 함수  
O	└── ...  
├── include/				# 헤더 파일  
O	├── system.h			# user 함수 선언  
O	├── kernel.h			# (yet)커널 관련 공통 헤더  
O	├── drivers/			# (yet)드라이버 헤더 파일  
O	└── lib/				# (yet)라이브러리 헤더 파일  
└── scripts/				# 빌드 및 유틸리티 스크립트  
O	├── build.sh			# 빌드 스크립트  
O	└── run_qemu.sh			# QEMU를 통해 OS 이미지 실행 스크립트  
