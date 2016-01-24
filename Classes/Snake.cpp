#include "Snake.h"

int Snake::get_current_dir() {
	log("get dir, nodes size = %d", snake_nodes->size());
	auto node = snake_nodes->back();
	log("get node");
	return node->dir;
}

void Snake::update(float dt) {
	log("snake updating~");
	auto u = UNIT;
	if (head_speed < EPS) {
		head_speed += add_speed;
		add_speed = 0;
	}
	head_step += head_speed;
	tail_step += tail_speed;
	if (head_step > 0) {
		head_step -= u;
		get_next_node();
		head_speed += add_speed;
		add_speed = 0;
		if (food_cache == 0) {
			tail_speed = head_speed;
		}
		else if (food_cache < 0) {
			tail_speed = 2 * head_speed;
			food_cache++;
		}
		else {
			tail_speed = 0;
			food_cache--;
		}
	}
	if (tail_step > 0) {
		tail_step -= u;
		remove_cache->push(snake_nodes->front());
		snake_nodes->pop();
	}
}

Snake::Snake(int x, int y, int dir) {
	log("start create a snake");
	snake_nodes = new queue<SnakeNode*>();
	remove_cache = new queue<SnakeNode*>();
	add_cache = new queue<SnakeNode*>();
	auto node = new SnakeNode(x, y, dir, unknown);
	snake_nodes->push(node);
	head_speed = 0;
	tail_speed = 0;
	head_step = 0;
	tail_step = 0;
	next_turn = 0;
	food_cache = 0;

	scheduler = new Scheduler();
	scheduler->scheduleUpdate(this, 3, false);
	log("creating the snake finished");
}

Snake::~Snake() {
	log("deleted a snake");
}

int Snake::get_length() {
	return snake_nodes->size();
}

void Snake::get_next_node() {
	snake_nodes->back()->type = next_turn;
	log("next turn = %d, type = %d", next_turn, snake_nodes->back()->type);
	auto node = new SnakeNode(*snake_nodes->back());
	node->type = unknown;
	node->dir += next_turn;
	node->dir &= DIR_MASK;
	next_turn = 0;
	switch (node->dir) {
	case DIRECTION::UP:
		node->y++;
		break;
	case DIRECTION::RIGHT:
		node->x++;
		break;
	case DIRECTION::DOWN:
		node->y--;
		break;
	case DIRECTION::LEFT:
		node->x--;
		break;
	default:
		break;
	}
	snake_nodes->push(node);
	add_cache->push(node);
}