# vshooter
## English
### Introduction.
Simple shoot-em-up game made using Raylib library. Your goal is to accmulate larger score and live longer.
You are controlling spaceship with abilty to shoot down enemies what come in your way.
You start with 5 lives (with ability to gain more after achieving specific score). Once you lose all lives it is game over.
High score is not saved on hard drive, it only exists only while program is beign run.
### Controls.
* Arrow keys - movement;
* Z key - fire;
* Tab key - view hitboxes;
### Enemies.
* Meteoroid - flies in random angle, deals only contact damage;
* UFO - moves in zig-zags and periodically fires blasters downwards;
* Enemy spaceships - moves straight down and periodically fires aimed plasma balls. Has 3 lives;
### Building
For compilation of program GNU Make is used. The only dependency is library called raylib. You need it's header (ends with .h) files and dynamic linking library files (.dll on Windows, .so on Linux) for your operating system you can get in [latest release](https://www.github.com/raysan5/raylib/releases) (For Windows: make sure release archive you are downloading has "mingw-w64" postfix). You need to put those files in project's directory. To compile on Windows you additionally need program called [w64devkit](https://www.github.com/skeeto/w64devkit/releases).
Compilation process on Linux looks like this:
```
cd vshooter
make
```
Also you can add g++ compiler flags through CFLAGS variable:
```
make CFLAGS="-O2 -mtune=native -march=native"
```
Compilation process on Windows through w64-devkit looks like this:
```
cd vshooter
make w64
```
### Credits.
* Ramon "raysan5" Santamaria - Developed Raylib library;
* https://tenor.com/ru/view/explos%C3%A3o-meme-gif-gif-5842472256224520495 - explosion animation;
* https://www.freepik.com/free-photo/grunge-wall-texture_988115.htm#fromView=keyword&page=1&position=0&uuid=23636918-ae8e-409d-8be9-db0c4bff9f2d&query=Metal+Texture - metal texture;
* Fonts Fantasque Sans Mono, Ume Font Gothic;
* Sound effects: mixkit.co, myinstants.com;

## На русском
### Введение.
Простая аркадная игра созданная при помощи библиотеки Raylib. Вашей целю является выживание и набор максимально возможного количества очков.
Вы контролируете космическим кораблем с возможностью отстреливать врагов.
Вы начинаете игру с 5 жизнями (существует возможность получить ещё по мере накопления очков). Игра завершается когда у вас жизней меньше нуля.
Рекорды не сохраняются на жёсткий диск, они существуют только во время работы программы.
### Управление.
* Стрелочки - движение;
* Клавиша Z - огонь;
* Клавиша Tab - просмотр хитбоксов;
### Враги.
* Метеороид - летит под случайным углом и наносит только контактный урон;
* НЛО - двигается зигзагами и периодически стреляет бластером вниз;
* Вражеский космический корабль - Двигается вниз и периодически стреляют бластером по игроку. У него 3 жизни;
### Компиляция
Для процесса сборки используется система сборки GNU Make. Единственной зависимостью является библиотека raylib. Нужно в директорию с проектом добавить заголовочные файлы (Заканчиваются .h) и файлы библиотеки динамической компоновки (на Windows .dll, на Linux .so) для своей операционной системы можно в [текущем релизе](https://www.github.com/raysan5/raylib/releases) (Пользователи Windows: убедитесь что скачиваемый архив имеет постфикс "mingw-w64"). Эти файлы следует поместить в директорию с проектом. Для компиляции проекта на Windows используется программа [w64devkit](https://www.github.com/skeeto/w64devkit/releases).
Процесс компиляции для Linux выглядит так:
```
cd vshooter
make
```
Также можно добавить флаги компилятора g++ через перменную CFLAGS:
```
make CFLAGS="-O2 -mtune=native -march=native"
```
Процесс компиляции на Windows через w64-devkit выглядит так:
```
cd vshooter
make w64
```
### Благодарность.
* Ramon "raysan5" Santamaria - разработал библиотеку Raylib;
* https://tenor.com/ru/view/explos%C3%A3o-meme-gif-gif-5842472256224520495 - анимация взрыва;
* https://www.freepik.com/free-photo/grunge-wall-texture_988115.htm#fromView=keyword&page=1&position=0&uuid=23636918-ae8e-409d-8be9-db0c4bff9f2d&query=Metal+Texture - металлическая текстура;
* Шрифты Fantasque Sans Mono, Ume Font Gothic;
* Звуковые эффекты: mixkit.co, myinstants.com;
