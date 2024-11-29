#include "the_bang.hpp"

using namespace Plteen;

/*************************************************************************************************/
void Plteen::TheBigBang::load(float width, float height) {
    this->title = this->insert(new Labellet(GameFont::Title(), this->title_color,
        "%s: %s", this->the_title_prefix(), this->name()));

    this->agent = this->insert(new Linkmon());
    this->agent->scale(-1.0F, 1.0F);

    this->title->camouflage(true);
    this->set_sentry_sprite(this->agent);
    
    this->tooltip = this->insert(make_label_for_tooltip(GameFont::Tooltip()));
    this->set_tooltip_matter(this->tooltip);
}

void Plteen::TheBigBang::reflow(float width, float height) {
    this->move_to(this->title, Position(this->agent, MatterPort::RB), MatterPort::LB);
}

/*************************************************************************************************/
float Plteen::TheBigBang::get_titlebar_height() {
    float height = 0.0F;

    if (this->agent != nullptr) {
        height = this->agent->get_bounding_box().height();
    }

    return height;
}
