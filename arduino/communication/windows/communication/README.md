Çalışma mantığı:
communication.ino dosyasındaki kod arduino board'a atılır.
TODO: 	rs232/demo_rx.c ve rs232/demo_tx.c dosyalarıyla communication.ino dosyasında belirlenen iletişim şekline uygun düzenleme yapılmalı.
		Visual koduna bu iki dosyadaki kodlar eklenecek.

***************

Arduino board can be transmit (Serial.write) or receive (Serial.read) from communication.ino

External programs are located at rs232/demo_rx.c (receive) and rs232/demo_tx.c (transmit). Build commands are located inside of those files.