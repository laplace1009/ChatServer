# ChatServer

### Windows Simple Chat Server

#### C++20 MSVC

##### Used Library
* 커스템 메모리 할당 라이브러리 사용
* 커스텀 ReadWriteLock 라이브러리 사용
* Windows Network Library 사용

##### Server
* 멀티스레드를 사용함
* LPFN 함수 시리즈를 사용하기 때문에 비동기 연산이며 IOCP로 응답을 받음
