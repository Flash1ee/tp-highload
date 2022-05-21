# Курс "Проектирование высоконагруженных систем"

**Тема курсовой работы:**  
Проектирование сервиса видеозвонков *Zoom*

## Введение в предметую область

[Zoom](https://ru.wikipedia.org/wiki/Zoom_(%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B0)) - проприетарная программа для организации видеоконференций, разработанная компанией Zoom Video Communications. Она предоставляет сервис видеотелефонии, который позволяет подключать одновременно до 100 устройств бесплатно, с 40-минутным ограничением для бесплатных аккаунтов.

В работе акцент будет сосредоточен на возможностях создания видеоконференций.

## Целевая аудитория

На декабрь 2020 года [[1.1]](#source-1) в ежедневных видеовстречах принимали участие около 350 миллионов пользователей.  
В 2021 году месячная аудитория сайта `zoom.us` в России была около 23 миллионов пользователей [[1.2]](#source-1).

Учитывая статистику 2020 года и то, что в СНГ приблизительно 2% пользователей `Zoom` можно высчитать, что количество пользователей = `350 * 0.02 ~= 7 млн.`.  
Это число показывает ежедневную аудиторию сервиса, однако, это не уникальные инспользования. Один человек может несколько раз участвовать в конференции.  
Пусть каждый человек в среднем делает по 3 звонка. Тогда `7 / 3 ~= 2 млн` => в СНГ ежедневная нагрузка сервиса может достигать 2 млн пользователей.  

Таким образом, целевая аудитория для проектируемого сервиса следующая:
- Страны СНГ;
- Пользователи в возрасте 18 - 50 лет;

Сводная таблица:  
| Месячная аудитория в СНГ | Еждневная нагрузка (кол-во сессий) | Дневная нагрузка по пользователям |
|--------------------------|------------------------------------|-----------------------------------|
| 23 млн                    | 7 млн                              | 2 млн                             |

### MVP
- Создание аккаунта, авторизация.
- Присоединиться к конференции.
- Создание видеоконференций:
  - Максимальное число участников - 50 человек.
  - Возможность приглашать/исключать участников.
  - Сохранение записей конференций.
  - Чат.
## Расчет нагрузки

### Продуктовые метрики
- Месячная аудитория - `23 млн.` чел.
- Дневная аудитория - `2 млн.` чел.
- Средний размер хранилища пользователя:
  - Хранение данных аккаунта `1 МБ` - фото пользователя, персональная информация.
  - Хранение записей видеозвонков (20% пользователей): ориентируемся, что пользователь сохраняет `5 часов` видео в месяц в хранилище сервиса. `1 час` видео в формате `HD` весит около `1 ГБ`, следовательно, за месяц пользователь тратит `5 ГБ`, за год `60 ГБ`.  
  - Хранение информации о запланированных конференциях:
   - 1 запись из `500 символов`, т.е `500 Б` * `8 Б` = `4000 Б`
- Среднее количество действий пользователя по типам за период (день)
1. **Регистрация / авторизация.**
В месяц пользователь 10 раз создает аккаунт или входит в него -> 0.33 действий в день.

2. **Создание конференции**
В среднем за месяц пользователь создает 30 конференций -> 30/30 = 1 создание в день.

3. **Участие в конференции (как участник)**
В среднем за месяц пользователь участвует в 60 конференциях -> 60/30 = 2 участия в день.

4. **Редактирование профиля**
В среднем пользователь редактирует свои данные 3 раза в месяц -> 3/30 = 0.1 редактирование в день

5. **Сохранение видеозаписи конференции**
В среднем пользователь сохраняет 5 записей в месяц -> 5/30 = 0.16 сохранений в день

6. **Отправка сообщений в чат**
В среднем пользователь отправляет 10 сообщений за конференцию -> 2 * 10 = 20 сообщений в день

**Сводная таблица**

**Итого**

| Регистрация и авторизация | Создание конференции | Участие в конференции | Редактирование профиля | Сохранение записи |  Отправка сообщений |
|---------------------------|----------------------|-----------------------|------------------------|-------------------|--------------------|
| 0.33                         | 1                  | 2                  | 0.1                    | 0.16              |20          |

#### Оценка аудитории

**Для СНГ**
- Дневная аудитория
    - Около `2 млн` человек уникальных пользователей
- Месячная аудитория
  - Desktop `24 миллиона` пользователей
### Технические метрики

**Замечания**

Пусть:
- в 1 конференции чат содержит в среднем `25000` символов или `4250` слов (примерно 10 страниц А4 12 шрифтом).
- строки хранятся в `utf-8` => `1 символ` занимает `8 байт`.
- в 1 сообщении `25 символов`.
- считаем, что сервисом польлзуется `25 миллионов` пользователей.
- одна сессия занимает `16` + `8 байт` = `24 байта` (sessionID, userID)
- хранилище звонков (информация об участниках конференции) - `32 байта` на запись.
- пользователь в среднем `60 %` времени конференции использует видеокамеру и `30 %` времени аудио
- средняя длительность `1` звонка = `54` минуты [[1.4]](#source-4), округлим до `1` часа

**Размер хранения**

  - Персональные данные (профиль) = `25.000.000` * `1Мб` = `23.84 Тб`
  - Записи конференций = `5 Гб` * `12 мес` * `25.000.000` = `4 394 531` ~= `1.5`миллионов Тб за год
  - Записи чатов = `25.000.000` * `2`(кол-во конференций в день) * `365` *  `25 000` * `8` * = `3318` ТБ за год
  - Хранилище сессий: `2 000 000` * `24` * `2` * `0.5` = `46` МБ.
  - Хранилища звонков: `2 000 000` × (`32 байта` * `2`) * `365` = `44 ГБ` в год.

**Сетевой трафик (по существенным типам)**

- По данным официального сайта Zoom [[1.5]](#source-5), для HD видео пропускная способность должна быть 1.8 МБит/с на загрузку, FULL HD - 3 МБит/с -> возьмем среднее (1.8 + 3) / 2 = 2.4 Мбит/с на пользователя
- 1 конференция длится 1 час -> пропускная способность на пользователя 2.4 * 3600 = 8640 Мбит/час = 8.64 ГБит/час
- Для всех пользователей: `8.64` *` 2 000 000` * `0.6` {60% времени используется видео} = `10 368 000` ГБит/ч
- Передача аудио = `16` Кбит/с * `10^(-6)` * `2 000 000` * `3600` * `0.3` = `34560` Гбит/ч
- Сохранение записи в облаке: По данным официального сайта Zoom [[1.5]](#source-5), на HD видео скорость выгрузки должна быть не менее 1.2 МБ/с
      - Для всех пользователей: `1.25` Мбит/с * `10^-3` * `2 000 000` * `0.16` = `400` Гбит/ч
**Сетевой трафик при пике нагрузки**

- Предположим, что в пиковый час нагрузка составляет 80% от дневной нагрузки, т.е. `1.6` миллион человек.  
- 80 % времени используется видео и звук.
- Пиковая нагрузка: `8.64` * `2 000 000 * 2ч / 24ч` * `0.8` + `16` Кбит/с * `10^(-6)` * `2 000 000 * 2ч / 24ч` * `3600` * `0.6` + `1.25` Мбит/с * `10^-3` * `2 000 000 * 2ч / 24ч` * `0.16` = `11,6` * `10^5` ГБит/ч = `322` ГБит/с


- **RPS в разбивке по типам запросов**

RPS можно посчитать по формуле: {число заходов на страницу одного человек} * 2 000 000 { число пользователь в день } / 24 / 60 / 60
- Регистрация и авторизация: `2 000 000` * `0.33` / 24 / 60 / 60 = 8 rps
- Создание и удаление конференции: `2 000 000` * `1` * 2 / 24 / 60 / 60 = 46 rps
- Участие в конференции: `2 000 000` * `2` * (`2` {получение звука и видео}) * `2` {присоединение, отсоединение} / 24 / 60 / 60 = 368 rps
- Редактирование профиля: `2 000 000` * `0.1` / 24 / 60 / 60 = 2.3 rps
- Сохранение видеозаписи: `2 000 000` * `0.16` / 24 / 60 / 60 = 3.7 rps
- Отправка сообщений в чат: `2 000 000` * `20` / 24 / 60 / 60 = 460 rps

Суммарно: `888 RPS`


## Логическая схема базы данных

![Highload zoom](https://user-images.githubusercontent.com/55987935/169652591-ead38916-0c67-4f61-a08c-d0499893e936.svg)

## Использованные источники
1. Статистические данные
    1. <a id="source-1"></a>[Статистические данные по zoom декабрь 2020](https://www.businessofapps.com/data/zoom-statistics/)
    2. <a id="source-2"></a>[Месячная статистика помещаемости в РФ](https://www.statista.com/statistics/1117659/average-traffic-on-remote-working-platforms-in-russia/)
    3. <a id="source-3"></a>[Статистические данные по zoom](https://backlinko.com/zoom-users)
    4. <a id="source-4"></a>[Данные по использованию Zoom в 2021](https://blog.zoom.us/how-you-zoomed-over-the-past-year-2021/)
