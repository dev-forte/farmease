(0) 설치한 노드
팔레트 설정 > 설치가능한 노드에서 검색하여 설치
1) node-red (4.0.5)
2) node-red-node-serialport (2.0.3)
3) node-red-node-mysql (2.0.0)
4) node-red-dashboard (3.6.5)
5) node-red-node-ui-table (0.4.4) (dashboard 설치 후 사용)


(1) 컴퓨터 재부팅 시
  1. DB 실행: 바탕화면의 Startdb.batch 파일 '*관리자 권한으로 실행'
     현재 사용 중인 DB 계정 설정은 바탕화면의 db비번.txt 파일 참조
  2. Node-RED 서버 실행: 바탕화면의 StartNodeRED.batch 파일 실행
  3. Node-RED 대시보드: http://localhost:1880/ui
  4. Node-RED 플로우 편집: http://localhost:1880/
     사용 전 COM 포트, DB 등 노드 '접속됨', 'OK' 등 작동 되는지 확인
     Node-RED 노드 파일은 USB 또는 github.com/dev-forte/farmease 첨부


(2) Node-RED 서버 재시작
  1. 실행 중인 node-red cmd 창 종료
  2. 바탕화면의 StartNodeRED.batch 파일 실행