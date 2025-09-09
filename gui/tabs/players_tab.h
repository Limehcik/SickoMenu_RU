#pragma once

namespace PlayersTab {
	const std::vector<const char*> FAKEROLES = { "Экипаж", "Предатель", "Ученый", "Инжинер", "Ангел Хранитель", "Оборотень", "Призрак Экипажа", "Призрак Предателя", "Паникер", "Фантом", "Следопыт" };
	const std::vector<const char*> GHOSTROLES = { "Ангел Хранитель", "Призрак Экипажа", "Призрак Предателя" };
	const std::vector<const char*> COLORS = { "Red", "Blue", "Green", "Pink", "Orange", "Yellow", "Black", "White", "Purple", "Brown", "Cyan", "Lime", "Maroon", "Rose", "Banana", "Gray", "Tan", "Coral", "Fortegreen" };
	const std::vector<const char*> SHIPVENTS = { "Admin", "Hallway", "Cafeteria", "Electrical", "Upper Engine", "Security", "Medbay", "Weapons", "Lower Reactor", "Lower Engine", "Shields", "Upper Reactor", "Upper Navigation", "Lower Navigation" };
	const std::vector<const char*> HQVENTS = { "Balcony", "Cafeteria", "Reactor", "Laboratory", "Office", "Admin", "Greenhouse", "Medbay", "Decontamination", "Locker Room", "Launchpad" };
	const std::vector<const char*> PBVENTS = { "Security", "Electrical", "O2", "Communications", "Office", "Admin", "Laboratory", "Lava Pool", "Storage", "Right Seismic", "Left Seismic", "Outside Admin" };
	const std::vector<const char*> AIRSHIPVENTS = { "Vault", "Cockpit", "Viewing Deck", "Engine", "Kitchen", "Upper Main Hall", "Lower Main Hall", "Right Gap Room", "Left Gap Room", "Showers", "Records", "Cargo Bay" };
	const std::vector<const char*> FUNGLEVENTS = { "Communications", "Kitchen", "Lookout", "Outside Dorm", "Laboratory", "Jungle (Laboratory)", "Jungle (Greenhouse)", "Splash Zone", "Cafeteria" };
	void Render();
}