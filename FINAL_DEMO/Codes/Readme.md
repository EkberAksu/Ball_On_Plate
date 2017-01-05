Arduino kodunu yüklerken gerekli kütüphaneleri yüklemediğiniz hatası alırsanız, Libraries  klasöründen bunları sisteminize ekleyiniz.
Bunun ardından arduino kodunu yüklemeye hazırsınız.
Final klasörü içindeki Final.ino yu yükleyiniz.

Ball&Plate klasörünü Clion ide'si ile açınız. Çalıştırmadan önce openscenegraph kütüphanesinin yüklü olup olmadığını kontrol ediniz. Eğer yüklü değilse :

#LINUX İÇİN

Linux için Open Scene Graph kütüphanesinin kurulumundan önce bağımlılıklarını kurmak isteyebilirsiniz. Bunun için şu linkteki adrese gidin:
http://trac.openscenegraph.org/projects/osg//wiki/Support/PlatformSpecifics/Debian-Dependencies

Bundan sonra şu komutları uygulayarak kütüphaneyi kurun:
# check out OSG
svn checkout http://svn.openscenegraph.org/osg/OpenSceneGraph/trunk OpenSceneGraph

# compile the OSG
cd OpenSceneGraph
./configure
make -j 8
cd ..

Bu komutlardan sonra uygulamanın cmakelist.txt dosyasında linux için openscenegraph pathini belirtin. Unutmayın bu path home/user klasöründeki değil usr/local/lib32 veya lib64 dizinleridir. Varsayılan olarak bu projede /usr/local/lib64/ kullanılmıştır.

#WINDOWS İÇİN

OpenSceneGraph ‘ın kurulması:
1. OpenSceneGraph klasörü C disk içerisindeki ProgramFiles(x86) ‘ın içine konulacaktır.
2. OpenSceneGraph klasörü içerisindeki bin klasörünün path’ i System Properties in içindeki Environment Variables path’ ine eklenecektir.
3. OpenSceneGraph\bin içerisindeki osgPlugins-3.4.0 klasörünün path’ i System Properties’ in içindeki Environment Variables path’ ine eklenecektir.

MinGW 0.6.0 ‘ın kurulması :
1. Bilgisayarda kurulu olan herhangi bir minGW sürümü varsa silenecek.
2. MinGW 0.6.0 installer indirilip bilgisayara kurulacak (link: https://sourceforge.net/projects/mingw/ ) .
3. Kurarken en azından şu package ler seçilmeli : gcc, g++, make, Cmake, gdb .
4.  MinGW\bin klasörünün path’i i System Properties in içindeki Environment Variables path ine eklenecektir.

JetBrains Clion’ın kurulması:
1. JetBrains Clion indirilip bilgisayara kurulacaktır (link : https://www.jetbrains.com/clion/download/#section=windows-version ).
2. Clion kurulduktan sonra şu şekilde toolchains açılacak : File -> Settings -> 'Build, Execution, Deployment' -> 'Toolchains'.
3. Envıronment kısmında MinGW’ın kuruldugu klasörün path’inin doğru gösterilmesi lazım.
4.  'CMake executable' kısmı için  'Use bundled CMake x.x.x'  ‘i seçilecek.
5. 'Debugger’ kısmı da  ' bundled debugger x.x.x'  ‘i göstermesi lazım.
6. Alt kısmında tüm kategoriler önünde doğru işareti olması ve  'make', 'C compiler' and 'C++ compiler' ların doğru path’I gösterilmiş olmsı lazım.
7. 'Run' -> 'Edit configuration' kısmına gittiğinizde projenizin ismi sol tarafta Configuratin da sağ tarafta gösterilecektir.
8. Target : all target, configuration: Debug, executabal: Ball&Plate(executable’un ismi) şeklinde ayarlanmış olacaktır.
9. ‘Working Directory’ kısmında projenin bulunduğu klasörün path’I seçilecektir.
10. CmakeList içeriği bu linkteki CmakeList ‘e değiştirilecek (link: https://github.com/alozta/395_Project1/blob/master/report/assets/CMakeList.txt ).