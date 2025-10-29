#include "Particle.h"

Particle::Particle(int id, float lifetime, sf::Vector2f velocity, sf::Color color)
	: id(id), lifetime(lifetime), color(color), has_been_updated(false)
{
	velocity = {5.f, 5.f};
}

void Particle::update()
{
}
