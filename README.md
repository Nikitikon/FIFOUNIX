# FIFOUNIX
Организовать серверный сокет на основе механизма FIFO (именованные каналы).
Особенности задания:
	•	Все клиентские приложения знают путь к серверному файлу FIFO.
	•	Подключаясь к серверу, клиент передает полный путь к собственному файлу FIFO.
	•	Получив от клиента имя файла, сервер создает дочерний процесс, который взаимодействует с клиентом через клиентский FIFO файл, родительский процесс при этом продолжает ожидать подключения новых клиентов.
	•	Дочерний процесс сервера начинает передавать клиенту произвольные символы, периодически меняя свой приоритет случайным образом от 0 до 10 (функция setpriority()).
	•	Клиент должен подсчитать, сколько символов сервер передал ему за 30 секунд.
	•	Через 30 секунд клиент должен корректно отключиться.