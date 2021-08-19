#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<string>
#include<map>
#include<assert.h>
#include<iostream>
#include<cstdlib>
using namespace std;
using namespace sf;

const int M = 20;//�������߶�
const int N = 10;//���������

struct Point //����
{
    int x, y;
}a[4], b[4], c[4], d[4];  //aΪ��ǰ���꣬bΪ���ݣ�cΪ��һ��,dΪ��½��
class Game {
public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game();
    bool load();//������Դ
    void run();//����
    //��ʼ����
    static int sco;//��ʼ����
    static int round;//��ʼ����
    const int Ez = 10;//����/�Ѷ�
    const int Mid = 20;
    const int Hd = 30;
    float DELAY = 0.3;
    int G = 0;//����,0��1��
    const  float QUICK = 0.05;//��down�����ٶ�
    const float INTERVAL = 0.07;//��ͬ�Ѷ�����ʱ����
    int field[M][N] = { 0 };//����������ʼΪ0��1Ϊ�з���
    int new_field[4][4] = { 0 };//��һ������չʾ��
private:
    //���ܺ��������
    void menu();//�˵�
    void processEvents();//�����¼�
    void update();//����֡
    void render();//��Ⱦ֡
    void newblock();//�·���
    void newnewblock();//Ԥ������
    bool check(Point*);//����Ƿ�Ϸ�,�������з�������
    bool lose();//�Ƿ����
    void gameover();//��������
    void coming_spot();//��������λ��
    void turn();//��ת
    void fall();//����
    void clear();//����
    void restart();//���ò���
    RenderWindow window;//����
    int start = 0;//�˵������ж��Ƿ�ʼ��Ϸ
    int dx = 0;//����λ��
    bool rotate = 0;//��ת
    int colorNum = 1;//ש����ɫ
    int newcolorNum = 1;//��һש����ɫ
    int flag_ac = 0;//���ٱ��
    float timer = 0, delay = DELAY;//��ʱ����������
    void acclerate();//���٣����ݲ���ȷ���ٶ�
    //��Դ����
    Clock clock;//��ʱ
    Sound sou_1, sou_2, sou_3, sou_4, sou_down, sou_gg;//��Ч
    Music menu_m, game_m;//����
    SoundBuffer smash_1, smash_2, smash_3, smash_4, ac, gg;//�ڴ���Ч
    Font font;//����
    Text score;//����
    Text ro;//����
    Texture t, t_ls, t_menu, t_bg, t_go;//����---t:��ͨש��,t_ls:�黯�����ʾש��,t_menu���˵�����
    //t_bg:��Ϸ����,t_go:gameover����
    Sprite s, ns, ls, mn, bg, go;//����------s:��ǰש�飬ns:��ש�飬ls:��½ש�飬mn:�˵�,bg:��Ϸ������go:gameover
    int figures[7][4] =  ///ש����״
    {
        1,3,5,7,//1��
        2,4,5,7,//S��
        3,5,4,6,//֮��
        3,5,4,7,//͹��
        2,3,5,7,//L��
        3,5,7,6,//J��
        2,3,4,5//������
        //////////////////////
        /* ---------������״
         | 0 | 1 |
         | 2 | 3 |
         | 4 | 5 |
         | 6 | 7 |
         ---------*/
         /////////////////
    };
};
Game::Game() :window(VideoMode(500, 800), "Tetris") {

}
int Game::sco = 0;
int Game::round = 0;
bool Game::load() {

    if (!game_m.openFromFile("audio/CJMG.ogg"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }

    if (!menu_m.openFromFile("audio/quiet.ogg"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }
    if (!font.loadFromFile("fonts/msyh.ttc"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }

    if (!smash_1.loadFromFile("audio/�ֶ�������.wav"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }
    if (!smash_2.loadFromFile("audio/��ͼ���Ҽ�ŭ.wav"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }
    if (!smash_3.loadFromFile("audio/��ȰͲ�.ogg"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }
    if (!smash_4.loadFromFile("audio/̫������.ogg"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }
    if (!ac.loadFromFile("audio/�ɲ����ҿ��.ogg"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }
    if (!gg.loadFromFile("audio/��������.wav"))
    {
        cout << "error" << endl;
        system("pause");
        return 0;
    }
    sou_1.setBuffer(smash_1);
    sou_2.setBuffer(smash_2);
    sou_3.setBuffer(smash_3);
    sou_4.setBuffer(smash_4);
    sou_down.setBuffer(ac);
    sou_gg.setBuffer(gg);

    t.loadFromFile("image/tiles35.png");
    t_ls.loadFromFile("image/tiles35light.png");
    t_menu.loadFromFile("image/menu.png");
    t_bg.loadFromFile("image/bg1.png");
    t_go.loadFromFile("image/gg.png");
    s.setTexture(t);
    ns.setTexture(t);
    ls.setTexture(t_ls);
    mn.setTexture(t_menu);
    bg.setTexture(t_bg);
    go.setTexture(t_go);
    s.setTextureRect(IntRect(0, 0, 35, 35));//IntRect(x,y,����)
    ns.setTextureRect(IntRect(0, 0, 35, 35));
    ls.setTextureRect(IntRect(0, 0, 35, 35));

    go.setPosition(78, 313);

    score.setFont(font);
    score.setString("0");
    score.setFillColor(Color(229, 140, 133));
    score.setPosition(24, 62);

    ro.setFont(font);
    ro.setString("0");
    ro.setFillColor(Color(229, 140, 133));
    ro.setPosition(24, 160);
    return true;
}

void Game::restart() {
    delay = DELAY;
    G = 0;
    start = 0;
    sco = 0;
    score.setString("0");
    round = 0;
    ro.setString("0");
    score.setPosition(24, 62);//��ʼ��������ʾλ��
    memset(field, 0, sizeof(field));//��ʼ��������
}

void Game::menu() {
    menu_m.play();//���Ų˵�����
    while (window.isOpen() && start == 0)
    {

        Event event;
        while (window.pollEvent(event) && start == 0)
        {

            if (event.type == Event::Closed)//�ر���Ϸ
                window.close();
            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Num1) { DELAY = 0.3; start = 1; menu_m.stop(); }
                else if (event.key.code == Keyboard::Num2) { DELAY = 0.2; start = 1; menu_m.stop(); }
                else if (event.key.code == Keyboard::Num3) { G = 1; start = 1; menu_m.stop(); }
                else if (event.key.code == Keyboard::Escape) { menu_m.stop(); window.close(); }

        }
        window.clear();
        window.draw(mn);
        window.display();
    }
}

void Game::turn() {
    if (rotate && colorNum != 7)//colorNum==7Ϊ������ש��
    {
        Point p = a[1];//��ת����
        for (int i = 0; i < 4; i++)
        {
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;

        }
        if (!check(a))
            for (int i = 0; i < 4; i++)
            {
                a[i] = b[i];
            }
    }
}

void Game::fall() {
    if (timer > delay)
    {
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].y += 1;

        }
        if (check(a) == false)//���
        {
            for (int i = 0; i < 4; i++)
            {
                field[b[i].y][b[i].x] = colorNum;
            }
            round++;
            ro.setString(to_string(round));
            newnewblock();//�����ʾδ�����ש��
            newblock();//�µ�����ש��
            coming_spot();//��ǰש�������״
        }
        //��ص�
        else
            for (int i = 0; i < 4; i++)
                d[i] = a[i];
        while (check(d) != false)
            for (int i = 0; i < 4; i++)
            {
                d[i].y += 1;
            }

        timer = 0;
    }
}

void Game::clear() {
    int k = M - 1;
    int combo = 0;
    for (int i = M - 1; i > 0; i--)
    {
        int count = 0;
        for (int j = 0; j < N; j++)
        {
            if (field[i][j])
                count++;
            field[k][j] = field[i][j];
        }
        if (count < N) k--;
        else
        {
            combo++;//����������������
        }

    }
    switch (combo)//��ͬ������ͬ��Ч
    {
    case 1:sou_1.play(); break;
    case 2:sou_2.play(); break;
    case 3:sou_3.play(); break;
    case 4:sou_4.play(); break;
    }
    sco += combo * combo;
    score.setString(to_string(sco));
}

bool Game::check(Point* a)
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
        else if (field[a[i].y][a[i].x]) return 0;//�����߽�
    return 1;
}

bool Game::lose()
{
    for (int i = 0; i < N; i++)
        if (field[1][i]) return 1;//�����߽�
    return 0;
}

void Game::coming_spot() {
    int n = colorNum - 1;
    for (int i = 0; i < 4; i++)
    {
        d[i].x = figures[n][i] % 2;
        d[i].y = figures[n][i] / 2;
    }
}

void Game::newnewblock() {
    if (round > 0)
        colorNum = newcolorNum;//��һ��Ԥ��ש����ɫ

    newcolorNum = 1 + rand() % 7;

    int n = newcolorNum - 1;
    //�����ת��������ֵ�����%2�õ�x���꣬���/2�õ�y���꣨ת��������������ô�����йأ�
    if (G)
        n = 0;
    for (int i = 0; i < 4; i++)
    {
        c[i].x = figures[n][i] % 2;
        c[i].y = figures[n][i] / 2;
    }
}

void Game::newblock() {

    //����,ע�͵��������У���ע�͵�����--------->7.22��ʵ�ֲ˵�ѡ�񿪹�ģʽ�����޸ĵ�355��n�ı俪��ģʽ������״
    if (round == 0)
        colorNum = 1 + rand() % 7;
    int n = colorNum - 1;
    //int n = 0;
    if (G)//����ģʽ,nȷ��ĳһ��״
        n = 0;
    //�����ת��������ֵ�����%2�õ�x���꣬���/2�õ�y���꣨ת��������������ô�����йأ�
    for (int i = 0; i < 4; i++)
    {
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
    }

}

void Game::acclerate() {
    if (round < Ez)
        delay = DELAY;
    else if (round < Mid)
        delay = DELAY - INTERVAL;
    else if (round < Hd)
        delay = DELAY - 2 * INTERVAL;
}

void Game::gameover() {

    Event event;
    sou_gg.play();
    score.setPosition(230, 410);//��ʾ�ڽ������
    score.setFillColor(Color(229, 140, 133));
    while (start == 1)//���й�menu()
    {
        while (window.pollEvent(event) && start == 1) {
            if (event.type == sf::Event::Closed)
                window.close();
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                start = 0;
            }
        }

        window.draw(go);
        window.draw(score);
        window.display();

    }
    game_m.stop();

}
void Game::run() {
    while (start == 0 && window.isOpen()) {
        restart();
        menu();
        game_m.setLoop(true);
        game_m.play();
        newnewblock();
        newblock();
        while (window.isOpen()) {

            processEvents();
            if (start == 0)break;
            update();
            if (lose())
            {
                gameover();
                game_m.stop();
                break;

            }
            render();
        }
        game_m.stop();
    }
}

void Game::processEvents() {
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)//�ر���Ϸ
            window.close();

        if (event.type == Event::KeyPressed)
            if (event.key.code == Keyboard::Up) rotate = true;
            else if (event.key.code == Keyboard::Left) dx = -1;
            else if (event.key.code == Keyboard::Right) dx = 1;
            else if (event.key.code == Keyboard::Escape)
            {
                gameover();
                game_m.stop();

            }
    }

    if (Keyboard::isKeyPressed(Keyboard::Down))
    {
        delay = QUICK;//��������
        flag_ac++;
        if (Event::EventType::KeyReleased)//������Ч
        {
            if (event.key.code == sf::Keyboard::Key::Down)
            {

                if ((flag_ac / 250.0) > 1)
                    sou_down.play();
                flag_ac = 0;
            }
        }
    }
    for (int i = 0; i < 4; i++)
    {
        b[i] = a[i];
        a[i].x += dx;
    }



}
void Game::update() {
    /// <- �ƶ� -> ///

    if (!check(a))//���Խ�磬����ԭλ
        for (int i = 0; i < 4; i++)
        {
            a[i] = b[i];
        }

    /// <- ��ת -> ///
    turn();

    for (int i = 0; i < 4; i++)//���ӵĻ���ʾ����ص��ש�鲻�ܺ����������ש��ˮƽͬ��
    {
        d[i].x = a[i].x;
    }

    ///�Զ�����///
    fall();

    ///����///
    clear();

    dx = 0; rotate = 0;
    acclerate();//���ݲ���ȷ���ٶ�
}
void Game::render() {
    ///draw///
    window.clear(Color::White);

    window.draw(bg);

    //����׻��۵ķ���
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
        {
            if (field[i][j] == 0)continue;
            s.setTextureRect(IntRect(field[i][j] * 35, 0, 35, 35));
            s.setPosition(j * 35, i * 35);
            s.move(140, 49);//offset
            window.draw(s);
        }
    for (int i = 0; i < 4; i++)
    {
        s.setTextureRect(IntRect(colorNum * 35, 0, 35, 35));//��ǰ����
        s.setPosition(a[i].x * 35, a[i].y * 35);//����λ
        s.move(140, 49);//����λ������
        window.draw(s);
    }
    //��ǰ����λ��
    for (int i = 0; i < 4; i++)
    {
        ls.setTextureRect(IntRect(colorNum * 35, 0, 35, 35));//��ǰ����
        ls.setPosition(d[i].x * 35, d[i].y * 35);//����λ
        ls.move(140, 14);//����λ������
        window.draw(ls);
    }
    //��һ������
    for (int i = 0; i < 4; i++)
    {
        ns.setTextureRect(IntRect(newcolorNum * 35, 0, 35, 35));
        ns.setPosition(c[i].x * 35, c[i].y * 35);//����λ
        ns.move(20, 250);//����λ����������Ļ���
        window.draw(ns);
    }

    window.draw(score);//��ʾ����

    window.draw(ro);//��ʾ����

    window.display();
}

int main() {
    Game game;
    if (game.load())
    {
        game.run();
        sleep(seconds(1));
    }
    else return -1;
    return 0;
}
