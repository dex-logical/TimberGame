// Подключаем важные библиотеки
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <sstream>
// Упрощаем использование кода с помощью "using namespace", 
// обозначающего пространство имен
using namespace sf;


const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//Где находится герой/ветка?
//Слева или справа?
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

//Объявление функции
void updateBranches(int seed) {
    //Переместите все ветки вниз на одно место
    for (int j = NUM_BRANCHES - 1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }

    //Создаем новую ветку в позиции 0
    //LEFT, RIGHT или NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r) {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    case 3:
        branchPositions[0] = side::NONE;
        break;
    }

}

int main()
{

    // Создаем объект VideoMode
    VideoMode vm(1920, 1080);
    // Создаем и открываем окно для игры в полноэкранном режиме
    RenderWindow window(vm, "Timber!", Style::Fullscreen);

    // Создаем текстуру для хранения изображения в GPU
    Texture textureBackground;

    // Загружаем текстуру
    textureBackground.loadFromFile("graphics/background.png");

    // Создаем спрайт
    Sprite spriteBackground;
    // Привязываем текстуру к спрайту
    spriteBackground.setTexture(textureBackground);

    // Устанавливаем позицию spriteBackground, чтобы он покрывал весь экран
    spriteBackground.setPosition(0, 0);

    //Создаем спрайт дерева
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    //Подготавливем пчелу
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    //Движется ли пчела в данный момент?
    bool beeActive = false;

    //С какой скоростью может лететь пчела?
    float beeSpeed = 0.0f;

    //Создаем три спрайта облаков из одной текстуры
    Texture textureCloud;

    //Загружаем новую текстуру
    textureCloud.loadFromFile("graphics/cloud.png");

    //Создаем облака с помощью массива
    const int NUM_CLOUDS = 6;
    Sprite clouds[NUM_CLOUDS];
    int cloudSpeeds[NUM_CLOUDS];
    bool cloudsActive[NUM_CLOUDS];

    for (int i = 0; i < NUM_CLOUDS; i++)
    {
        clouds[i].setTexture(textureCloud);
        clouds[i].setPosition(-300, i * 150);
        cloudsActive[i] = false;
        cloudSpeeds[i] = 0;

    }


    //Три новых спрайта с одной текстурой
    //Sprite spriteCloud1;
    //Sprite spriteCloud2;
    //Sprite spriteCloud3;
    //spriteCloud1.setTexture(textureCloud);
    //spriteCloud2.setTexture(textureCloud);
    //spriteCloud3.setTexture(textureCloud);

    ////Размещаем облака в левой части экрана на разной высоте
    //spriteCloud1.setPosition(0, 0);
    //spriteCloud2.setPosition(0, 150);
    //spriteCloud3.setPosition(0, 300);

    ////Выясняем, находятся ли облака в данный момент на экране
    //bool Cloud1Active = false;
    //bool Cloud2Active = false;
    //bool Cloud3Active = false;

    ////С какой скоростью движется каждое облако
    //float cloud1Speed = 0.0f;
    //float cloud2Speed = 0.0f;
    //float cloud3Speed = 0.0f;

    //Переменные для управления временем
    Clock clock;

    //Временная шкала
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    //Отслеживание состояния игры (на паузе или нет)
    bool paused = true;

    //Отрисовка текста
    int score = 0;

    Text messageText;
    Text scoreText;

    //Подключаем шрифт
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    //Устанавливаем шрифт
    messageText.setFont(font);
    scoreText.setFont(font);

    //Устанавливаем текст
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    //Устанавливаем размер сообщения
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    //Выбираем цвет
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    //Позиционируем текст
    FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top +
        textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    //Подготавливем 5 ветвей
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    //Устанавливаем текстуру для каждого спрайта ветки
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);

        //Задаем точку привязки спрайта
        //Это позволит вращать его, не меняя его положения
        branches[i].setOrigin(220, 20);
    }

    //Подготовка игрового персонажа
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    //Игрок начинает слева
    side playerSide = side::LEFT;

    //Подготовка надгробия
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    //Подготовка топора
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    //Выравниваем топор относительно дерева
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //Подговтока отлетающего бревна
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    //Некоторые другие полезные переменные
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    //Управление вводом
    bool acceptInput = false;

    //Подготовка звуков
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    //Время вышло
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    while (window.isOpen())
    {

        /*
        ******************************
           Обработка ввода игрока
        ******************************
         */

        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::KeyReleased && !paused) {
                //Снова слушаем нажатия клавиш
                acceptInput = true;

                //Скрываем топор
                spriteAxe.setPosition(2000,
                    spriteAxe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        //Старт игры
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
            //Сброс времени и счета
            score = 0;
            timeRemaining = 6;

            //Убираем все ветки
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            //Скрываем надгробие
            spriteRIP.setPosition(675, 2000);

            //Перемещаем героя в начальную позицию
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

        //Оборачиваем элементы управления персонажем,
        //Чтобы убедиться, что мы принимаем ввод
        if (acceptInput) {
            //Потенциально много кода...

            if (Keyboard::isKeyPressed(Keyboard::Right) or Keyboard::isKeyPressed(Keyboard::D))
            {
                //Убеждаемся, что персонаж справа
                playerSide = side::RIGHT;

                score++;

                //Добавляем доп время
                timeRemaining += (2.0 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT,
                    spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200, 720);

                //Обновление веток
                updateBranches(score);

                //Запускаем бревно влево
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                //Воспроизводим звук
                chop.play();
            }
            //Обработка нажатия клавиши со стрелкой влево
            if (Keyboard::isKeyPressed(Keyboard::Left) or Keyboard::isKeyPressed(Keyboard::A))
            {
                //Убеждаемся, что персонаж слева
                playerSide = side::LEFT;

                score++;

                //Добавляем доп время
                timeRemaining += (2.0 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT,
                    spriteAxe.getPosition().y);

                spritePlayer.setPosition(580, 720);

                //Обновление веток
                updateBranches(score);

                //Запускаем бревно влево
                spriteLog.setPosition(810, 720);

                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                //Воспроизводим звук
                chop.play();
            }
        }

        /*
        ************************
           Обновление сцены
        ************************
         */


        if (!paused) {

            //Измеряем время
            Time dt = clock.restart();

            //Уменьшаем оставшееся время
            timeRemaining -= dt.asSeconds();
            //Изменяем размер временной шкалы
            timeBar.setSize(Vector2f(timeBarWidthPerSecond *
                timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                //Ставим игру на паузу
                paused = true;

                //Меняем сообщение для игрока
                messageText.setString("OUT OF TIME!");

                //Пересчитываем позицию текста на основе его нового размера
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top +
                    textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                // Воспроизводим звук окончания времени
                outOfTime.play();
            }

            //Настройка пчелы
            if (!beeActive)
            {
                //Задаем скорость пчелы
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                //Задаем высоту полета пчелы
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }

            else
                //Движение пчелы
            {
                spriteBee.setPosition(
                    spriteBee.getPosition().x -
                    (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y);

                //Достигла ли пчела левого края экрана?
                if (spriteBee.getPosition().x < -100)
                {
                    //Готовим её к появлению в качестве новой пчелы в следующем кадре
                    beeActive = false;
                }
            }

            for (int i = 0; i < NUM_CLOUDS; i++)
            {
                if (!cloudsActive[i])
                {
                    // How fast is the cloud
                    srand((int)time(0) * i);
                    cloudSpeeds[i] = (rand() % 200);

                    // How high is the cloud
                    srand((int)time(0) * i);
                    float height = (rand() % 150);
                    clouds[i].setPosition(-200, height);
                    cloudsActive[i] = true;

                }
                else
                {
                    // Set the new position
                    clouds[i].setPosition(
                        clouds[i].getPosition().x +
                        (cloudSpeeds[i] * dt.asSeconds()),
                        clouds[i].getPosition().y);

                    // Has the cloud reached the right hand edge of the screen?
                    if (clouds[i].getPosition().x > 1920)
                    {
                        // Set it up ready to be a whole new cloud next frame
                        cloudsActive[i] = false;
                    }

                }

            }

            //Управление облаками
            //Облако 1
            //if (!Cloud1Active) {
            //    //Задаем скорость облака
            //    srand((int)time(0) * 10);
            //    cloud1Speed = (rand() % 200);

            //    //Задаем высоту облака
            //    srand((int)time(0) * 10);
            //    float height = (rand() % 150);
            //    spriteCloud1.setPosition(-200, height);
            //    Cloud1Active = true;
            //}

            //else {
            //    spriteCloud1.setPosition(
            //        spriteCloud1.getPosition().x +
            //        (cloud1Speed * dt.asSeconds()),
            //        spriteCloud1.getPosition().y);
            //    //Достигло ли облако правого края экрана?
            //    if (spriteCloud1.getPosition().x > 1920)
            //    {
            //        //Готовим его к появлению в качестве нового облака в следующем кадре
            //        Cloud1Active = false;
            //    }
            //}

            ////Облако 2
            //if (!Cloud2Active) {
            //    //Задаем скорость облака
            //    srand((int)time(0) * 20);
            //    cloud2Speed = (rand() % 200);

            //    //Задаем высоту облака
            //    srand((int)time(0) * 20);
            //    float height = (rand() % 300) - 150;
            //    spriteCloud2.setPosition(-200, height);
            //    Cloud2Active = true;
            //}

            //else {
            //    spriteCloud2.setPosition(
            //        spriteCloud2.getPosition().x +
            //        (cloud2Speed * dt.asSeconds()),
            //        spriteCloud2.getPosition().y);
            //    //Достигло ли облако правого края экрана?
            //    if (spriteCloud2.getPosition().x > 1920)
            //    {
            //        //Готовим его к появлению в качестве нового облака в следующем кадре
            //        Cloud2Active = false;
            //    }
            //}

            ////Облако 3
            //if (!Cloud3Active) {
            //    //Задаем скорость облака
            //    srand((int)time(0) * 30);
            //    cloud3Speed = (rand() % 200);

            //    //Задаем высоту облака
            //    srand((int)time(0) * 30);
            //    float height = (rand() % 450) - 150;
            //    spriteCloud3.setPosition(-200, height);
            //    Cloud3Active = true;
            //}

            //else {
            //    spriteCloud3.setPosition(
            //        spriteCloud3.getPosition().x +
            //        (cloud3Speed * dt.asSeconds()),
            //        spriteCloud3.getPosition().y);
            //    //Достигло ли облако правого края экрана?
            //    if (spriteCloud3.getPosition().x > 1920)
            //    {
            //        //Готовим его к появлению в качестве нового облака в следующем кадре
            //        Cloud3Active = false;
            //    }
            //}

            //Обновление текста счета
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            //Обновление спрайтов веток
            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;

                if (branchPositions[i] == side::LEFT)
                {
                    //Перемещаем спрайт влево
                    branches[i].setPosition(610, height);

                    //Поворачиваем спрайт в левую сторону
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    //Перемещаем спрайт в правую сторону
                    branches[i].setPosition(1330, height);

                    //Переворачиваем спрайт в другую сторону
                    branches[i].setRotation(0);
                }

                else
                {
                    //Скрываем ветку
                    branches[i].setPosition(3000, height);
                }
            }

            //Обработка отлетающего бревна
            if (logActive)
            {
                spriteLog.setPosition(
                    spriteLog.getPosition().x +
                    (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y +
                    (logSpeedY * dt.asSeconds()));

                //Достигло ли бревно правого края?
                if (spriteLog.getPosition().x < -100 ||
                    spriteLog.getPosition().x > 2000)
                {
                    //Готовим его к появлению в качестве нового бревна в следующем кадре
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }
            //Был ли персонаж раздавлен веткой?
            if (branchPositions[5] == playerSide) {
                //Смерть
                paused = true;
                acceptInput = false;

                //Отрисовка надгробия
                spriteRIP.setPosition(520, 760);

                //Скрываем персонажа
                spritePlayer.setPosition(2000, 660);

                //Изменяем текст сообщения
                messageText.setString("SQUISHED!");

                //Центрируем сообщение
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                // Воспроизводим звук, означающий гибель персонажа
                death.play();
            }
        }



        /*
        ***********************
           Отрисовка сцены
       ***********************
        */

        //Очищаем всё с предыдущего кадра
        window.clear();

        //Отрисовываем здесь нашу игровую сцену
        window.draw(spriteBackground);

        ////Отрисовка облаков
        //window.draw(spriteCloud1);
        //window.draw(spriteCloud2);
        //window.draw(spriteCloud3);

        for (int i = 0; i < NUM_CLOUDS; i++)
        {
            window.draw(clouds[i]);
        }

        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(branches[i]);
        }

        //Отрисовка дерева
        window.draw(spriteTree);

        //Отрисовка персонажа
        window.draw(spritePlayer);

        //Отрисовка топора
        window.draw(spriteAxe);

        //Отрисовка отлетающего топора
        window.draw(spriteLog);

        //Отрисовка надгробия
        window.draw(spriteRIP);

        //Отрисовка пчелы
        window.draw(spriteBee);

        //Отрисовка счета
        window.draw(scoreText);

        //Отрисовка временной шкалы
        window.draw(timeBar);

        if (paused) {
            //Отрисовка сообщения
            window.draw(messageText);
        }

        //Отображаем всё, что было отрисовано
        window.display();

    }

    return 0;
}