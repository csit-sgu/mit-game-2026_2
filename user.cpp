void DrawStatus(Context& ctx)
{
    const float panel_height = 85.0f;
    const float panel_padding = 25.0f;
    const float heart_size = 35.0f;

    int screen_width = GetScreenWidth();

    // Полупрозрачный фон с градиентом
    for (int y = 0; y < panel_height; y++)
    {
        float alpha = 0.7f - (y / panel_height) * 0.3f;
        DrawLine(0, y, screen_width, y,
            ColorAlpha(BLACK, alpha));
    }

    // Декоративная рамка снизу
    DrawRectangle(0, panel_height - 3, screen_width, 3, BLUE);

    // Жизни с анимацией 
    Texture heart_texture = ctx.textures_storage[ctx.heart->hash];
    float heart_x = panel_padding;
    float heart_y = (panel_height - heart_size) / 2;

    for (int i = 0; i < ctx.lives; i++)
    {
        // Пульсация при малом количестве жизней
        Color heart_color = WHITE;
        float scale = 1.0f;

        if (ctx.lives <= 2 && ctx.lives > 0)
        {
            float pulse = sin(ctx.time * 0.008f) * 0.1f + 0.9f;
            scale = pulse;
            heart_color = ColorAlpha(RED, 0.8f + pulse * 0.2f);
        }

        Rectangle dest_rect =
        {
            heart_x,
            heart_y + (heart_size * (1 - scale)) / 2,
            heart_size * scale,
            heart_size * scale
        };

        DrawTexturePro(heart_texture,
            { 0, 0, (float)heart_texture.width, (float)heart_texture.height },
            dest_rect, { 0, 0 }, 0, heart_color);

        heart_x += heart_size + 12;
    }

    // Счет
    char score_text[64];
    sprintf(score_text, "%d", ctx.score);

    // Тень для текста
    DrawText("SCORE:", screen_width / 2 - 100, panel_height / 2 - 20, 20,
        ColorAlpha(WHITE, 0.5f));
    DrawText(score_text, screen_width / 2 - 80, panel_height / 2 - 15, 40,
        ColorAlpha(BLACK, 0.5f));
    DrawText(score_text, screen_width / 2 - 82, panel_height / 2 - 17, 40,
        YELLOW);

    // Время с таймером
    int total_seconds = ctx.time / 1000;
    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;

    char time_text[64];
    sprintf(time_text, "%02d:%02d", minutes, seconds);

    // Иконка часов 
    float clock_x = screen_width - 130;
    float clock_y = panel_height / 2 - 15;
    DrawCircle(clock_x + 15, clock_y + 15, 18, ColorAlpha(WHITE, 0.3f));
    DrawCircle(clock_x + 15, clock_y + 15, 15, ColorAlpha(BLACK, 0.5f));

    float angle = (total_seconds % 60) * 6 * DEG2RAD;
    DrawLineEx({ clock_x + 15, clock_y + 15 },
        { clock_x + 15 + cos(angle) * 10, clock_y + 15 + sin(angle) * 10 },
        2, WHITE);

    DrawText(time_text, clock_x + 40, clock_y + 5, 30,
        total_seconds < 10 ? RED : WHITE);

    // Прогресс бар времени 
    const int TIME_LIMIT = 300;   

    if (total_seconds > 0 && total_seconds < TIME_LIMIT)
    {
        float progress = 1.0f - (float)total_seconds / TIME_LIMIT;
        DrawRectangle(screen_width - 200, panel_height - 5,
            200 * progress, 3,
            ColorAlpha(RED, 0.7f));
    }
