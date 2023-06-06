# Тестирование интерпретируемых программ

__Небольшое предисловие:__ Эта часть проекта появилась из-за того, что сам способ тестирования компилируемых и интерпретируемых программ разный. Мы не можем контроллировать, какие системные вызовы дёргает интерпретируемая программа (По крайне мере такого способа найдено не было).

![Схема проекта](https://downloader.disk.yandex.ru/preview/f56249a46f2baa8536dacbad3f2397a25a00a0a72b8c5b8a672cf914bf2d0c6c/647f2945/_r9GfakP7CVhvHeV-iWZTQs4q7ijn_xK25cmlvBcK7GfruP1J6XPXHzBTpZJiplT3Vj3rz3pFmoQtVtfHjbSAA%3D%3D?uid=0&filename=photo_2023-03-26_16-40-56.jpg&disposition=inline&hash=&limit=0&content_type=image%2Fjpeg&owner_uid=0&tknv=v2&size=2048x2048)
    
------
    
### Чеклист реализации:
  - [x] Создать клиентский синхронный сервер 
  - [x] Создать синхронный сервер, который будет запускать программу внутри докер-контейнера и возвращать результат
  - [ ] Написать механизм создания докер-контейнера и передачу в него программы (Внизу разные варианты реализации)
    1. Перед создание контейнера копировать в папку нужную программу и её "копировать" при создании контейнера вместе с комнтроллирующей программой 
    2. Создать volume-папку (общую папку для докер-контейнера и системы, из которой ты запускаешься. То же самое, что и общая папка в виртуалке), которую будем маунтить ко всем докер-контейнерам, туда копировать нужную программу, а её путь передавать через клиент-сервер программе внутри докера. При такой реализации input.txt также можно передавать вместе с нужной программой, меньше заморочек с использованием порта.
  - [ ] Написать кастомные ограничения для докер-контейнеров. В докер-контейнерах можно [создавать ограничения, на выполнение по памяти, ядрам, потокам и тд](https://docs.docker.com/config/containers/resource_constraints/). Нужно разобраться, как из DockerClient запускать контейнеры с разными ограничениями. 



## Зависимости
- [Docker](https://docs.docker.com/engine/install/ubuntu/)
- [DockerClient](https://github.com/enabled-robotics/DockerClient/tree/84f3c03304e1b75da19d5597c9f36740b066c337)
- [Boost](https://stackoverflow.com/questions/12578499/how-to-install-boost-on-ubuntu)
