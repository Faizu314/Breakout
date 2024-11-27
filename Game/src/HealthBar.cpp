#include "HealthBar.hpp"

void HealthBar::Start() {
    m_Heart1 = m_Entity->GetChild(0).lock();
    m_Heart2 = m_Entity->GetChild(1).lock();
    m_Heart3 = m_Entity->GetChild(2).lock();
}

void HealthBar::OnLifeChanged(int currLife) {
    m_Heart1->GetRenderData()->Tint = currLife < 1 ? Phezu::Color::Red : Phezu::Color::Green;
    m_Heart2->GetRenderData()->Tint = currLife < 2 ? Phezu::Color::Red : Phezu::Color::Green;
    m_Heart3->GetRenderData()->Tint = currLife < 3 ? Phezu::Color::Red : Phezu::Color::Green;
}
