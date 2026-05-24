static void DrawTextWithShadow(
    const char *text,
    int x,
    int y,
    int font_size,
    Color text_color,
    Color shadow_color,
    int shadow_offset = 2)
{
    DrawText(
        text,
        x + shadow_offset,
        y + shadow_offset,
        font_size,
        shadow_color
    );

    DrawText(
        text,
        x,
        y,
        font_size,
        text_color
    );
}

void DrawStatus(Context &ctx)
{
    // Размеры панели
    const float panel_height = 85.0f;
    const float panel_padding = 25.0f;

    // Сердца
    const float heart_size = 35.0f;
    const float heart_spacing = 12.0f;

    // Пульсация
    const float pulse_amplitude = 0.1f;
    const float pulse_base_scale = 0.9f;
    const float pulse_period_ms = 800.0f;

    // Таймер
    const int time_limit_seconds = 300;

    // Часы
    const int clock_x_offset = 130;
    const float clock_radius_outer = 18.0f;
    const float clock_radius_inner = 15.0f;
    const float clock_hand_length = 10.0f;

    // Прогресс-бар
    const int progress_bar_width = 200;
    const int progress_bar_height = 3;

    int screen_width = GetScreenWidth();

    //
    // Фон
    //

#if defined(RAYLIB_VERSION_MAJOR) && (RAYLIB_VERSION_MAJOR >= 4)

    DrawRectangleGradientV(
        0,
        0,
        screen_width,
        panel_height,
        ColorAlpha(BLACK, 0.7f),
        ColorAlpha(BLACK, 0.4f)
    );

#else

    for (int y = 0; y < panel_height; y++)
    {
        float alpha =
            0.7f - ((float)y / panel_height) * 0.3f;

        DrawLine(
            0,
            y,
            screen_width,
            y,
            ColorAlpha(BLACK, alpha)
        );
    }

#endif

    // Нижняя рамка
    DrawRectangle(
        0,
        panel_height - progress_bar_height,
        screen_width,
        progress_bar_height,
        BLUE
    );

    //
    // проверка текстуры сердца 
    //

    if (ctx.heart == nullptr)
    {
        return;
    }

    auto texture_it =
        ctx.textures_storage.find(ctx.heart->hash);

    if (texture_it == ctx.textures_storage.end())
    {
        return;
    }

    Texture heart_texture = texture_it->second;

    //
    // Жизни
    //

    float heart_x = panel_padding;
    float heart_y =
        (panel_height - heart_size) * 0.5f;

    for (int i = 0; i < ctx.lives; i++)
    {
        Color heart_color = WHITE;
        float scale = 1.0f;

        if (ctx.lives <= 2)
        {
            float pulse =
                sinf(
                    2.0f * PI *
                    ctx.time /
                    pulse_period_ms
                ) *
                pulse_amplitude +
                pulse_base_scale;

            scale = pulse;

            heart_color =
                ColorAlpha(
                    RED,
                    0.8f + pulse * 0.2f
                );
        }

        Rectangle source_rect =
        {
            0.0f,
            0.0f,
            (float)heart_texture.width,
            (float)heart_texture.height
        };

        Rectangle dest_rect =
        {
            heart_x,
            heart_y +
                (heart_size * (1.0f - scale)) * 0.5f,
            heart_size * scale,
            heart_size * scale
        };

        DrawTexturePro(
            heart_texture,
            source_rect,
            dest_rect,
            {0.0f, 0.0f},
            0.0f,
            heart_color
        );

        heart_x += heart_size + heart_spacing;
    }

    //
    // Счет
    //

    char score_text[64];

    snprintf(
        score_text,
        sizeof(score_text),
        "%d",
        ctx.score
    );

    DrawTextWithShadow(
        "SCORE:",
        screen_width / 2 - 100,
        panel_height / 2 - 20,
        20,
        WHITE,
        ColorAlpha(BLACK, 0.5f)
    );

    DrawTextWithShadow(
        score_text,
        screen_width / 2 - 82,
        panel_height / 2 - 17,
        40,
        YELLOW,
        ColorAlpha(BLACK, 0.5f)
    );

    //
    // Время
    //

    int total_seconds =
        ctx.time / 1000;

    int minutes =
        total_seconds / 60;

    int seconds =
        total_seconds % 60;

    char time_text[64];

    snprintf(
        time_text,
        sizeof(time_text),
        "%02d:%02d",
        minutes,
        seconds
    );

    float clock_x =
        screen_width - clock_x_offset;

    float clock_y =
        panel_height * 0.5f - 15.0f;

    // Внешний круг
    DrawCircle(
        clock_x + 15,
        clock_y + 15,
        clock_radius_outer,
        ColorAlpha(WHITE, 0.3f)
    );

    // Внутренний круг
    DrawCircle(
        clock_x + 15,
        clock_y + 15,
        clock_radius_inner,
        ColorAlpha(BLACK, 0.5f)
    );

    // Стрелка секунд
    float angle =
        (float)(total_seconds % 60) *
        6.0f *
        DEG2RAD;

    Vector2 clock_center =
    {
        clock_x + 15,
        clock_y + 15
    };

    Vector2 hand_end =
    {
        clock_center.x +
            cosf(angle) * clock_hand_length,

        clock_center.y +
            sinf(angle) * clock_hand_length
    };

    DrawLineEx(
        clock_center,
        hand_end,
        2.0f,
        WHITE
    );

    DrawTextWithShadow(
        time_text,
        clock_x + 40,
        clock_y + 5,
        30,
        total_seconds < 10 ? RED : WHITE,
        ColorAlpha(BLACK, 0.5f)
    );

    //
    // Прогресс бар времени
    //

    if (total_seconds < time_limit_seconds)
    {
        float progress =
            (float)total_seconds /
            (float)time_limit_seconds;

        DrawRectangle(
            screen_width - progress_bar_width,
            panel_height - progress_bar_height,
            (int)(progress_bar_width * progress),
            progress_bar_height,
            ColorAlpha(RED, 0.7f)
        );
    }
}
