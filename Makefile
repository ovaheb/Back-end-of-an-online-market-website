a.out: main.o product.o car.o mobile.o tv.o user.o admin.o buyer.o seller.o
	g++ -std=c++11 main.o product.o car.o mobile.o tv.o user.o admin.o buyer.o seller.o
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
manager.o: manager.cpp
	g++ -std=c++11 -c manager.cpp
product.o: product.cpp
	g++ -std=c++11 -c product.cpp
car.o: car.cpp
	g++ -std=c++11 -c car.cpp
mobile.o: mobile.cpp
	g++ -std=c++11 -c mobile.cpp
tv.o: tv.cpp
	g++ -std=c++11 -c tv.cpp
user.o: user.cpp
	g++ -std=c++11 -c user.cpp
admin.o: admin.cpp
	g++ -std=c++11 -c admin.cpp
buyer.o: buyer.cpp
	g++ -std=c++11 -c buyer.cpp
seller.o: seller.cpp
	g++ -std=c++11 -c seller.cpp
clean:
	rm -f *.o