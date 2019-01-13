# AES
Explanation
---
Encrypted with a program that encrypts the entire file in AES mode by reading the desired file as binary value, the original file name and extension name are appended with the extension aes, and the original file is deleted. If an encrypted file is selected, it will only proceed if the extension of the file is aes, and the encrypted file will be deleted.

Dependency
---
Developed in C++/MFC for Visual Studio 2017 in windows 10 environment, used OPENSSL

Generate Keys
---
The user-input key is used as the key through the md5 hash operation.

How to Use
---
1. Select File
2. Enter the encryption key
3. Encryption via the Encryption button
4. Abolition through the ventriloquize button

AES
---

설명
---
원하는 파일을 바이너리값으로 읽어서 파일 전체를 AES방식으로 암호화 하는 프로그램으로 암호화된 파일은 원래 파일명과 확장자 명 뒤에 aes라는 확장자명이 추가로 붙으며 원본 파일은 삭제가 된다. 복호화는 암호화된 파일을 선택했을 시 파일의 확장자명이 aes일 경우에만 진행이 되며, 암호화 된 파일은 삭제가 된다.

의존성
---
windows 10환경, Visual Studio 2017에서 C++/MFC 및 OPENSSL을 사용했다.

키 생성
---
사용자가 입력한 키를 md5 해시연산을 통한 값을 키로 사용한다.

사용법
---
1. 파일선택
2. 암호키 입력
3. 암호화 버튼을 통해 암호화
4. 복호화 버튼을 통해 복호화
