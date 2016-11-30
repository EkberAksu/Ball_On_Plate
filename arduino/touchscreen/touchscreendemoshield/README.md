Sağlıklı değerler veren touchscreen projesi.
Basınç bilgisi bu bağlantı şekilleriyle okunmuyor(0). Farklı bağlantı şekilleriyle de basınç okunurken x,y okunamadı.


			Board görünümü:

							-		-



							-		-
								||	(model kodu)
							(kablo yazısı diğer tarafa bakacak şekilde)



			(max x, min y)	-		- (min x,y) ORIJIN



				(max x,y)	-		-(min x, max y)
								||

			Bağlantılar:
				-Ekran: 4G yazısı arkaya bakacak şekilde soldan sağa sırasıyla
					BEYAZ
					KIRMIZI
					YEŞİL ~~KAHVERENGİ~~
					SİYAH(KAHVERENGİ) ~~YEŞİL~~
				-Board:
					BEYAZ 		-> A0	(analog)
					KIRMIZI 	-> A1	(analog)
					YEŞİL 		-> 3	(digital) ~~KAHVERENGİ~~
					SİYAH(KHV)	-> 4	(digital) ~~YEŞİL~~

Notlar:
-Ekran üzerinde bir şey yokken (0,1023) değeri alınıyor.
-x ve y için <100, >1000 değerleri ihmal edilsin. Ara değerler üzerinden servo tepkisi üretilsin.

-Board üzerinde max,min x,y testleri yaparken her iki eksen için de
	ORTALAMA MIN 120
	ORTALAMA MAX 880(x için), 950(y için)
olduğu görüldü.
