void OnShootButtonPressed()
{
    auto bullet = Object();
    
    bullet.position = player.position;
    
    auto render = Render("Assets2/bullet.png");
    bullet.render = render;
    
    auto collider = Collider(bullet.render, ColliderType::EVENT);
    bullet.collider = collider;

    float bullet_speed = 500.0f; 
    float velocityX = 0.0f;
    
    if (player.player.direction == Direction::LEFT)
    {
        velocityX = -bullet_speed;
    }
    else if (player.player.direction == Direction::RIGHT)
    {
        velocityX = bullet_speed;
    }
    
    float bullet_life_time = 3.0f; 
    auto bullet_params = Bullet(velocityX, bullet_life_time);
    bullet.bullet = bullet_params;
    
    Spawn(bullet);
}