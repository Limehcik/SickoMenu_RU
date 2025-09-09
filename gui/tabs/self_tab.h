#pragma once

namespace SelfTab {
	const std::vector<const char*> TPOPTIONS = { "None", "Radar", "Anywhere" };
	const std::vector<const char*> FAKEROLES = { "Экипаж", "Предатель", "Ученый", "Инжинер", "Ангел Хранитель", "Оборотень", "Призрак Экипажа", "Призрак Предателя", "Паникер", "Фантом", "Следопыт" };
	const std::vector<const char*> NAMEGENERATION = { "Комбинация Слов", "Рандом Буквы", "Цикл Ников" };
	const std::vector<const char*> BODYTYPES = { "Нормальный", "Лошадь", "Длинный" };
	const std::vector<const char*> FONTS = { "Barlow-Italic", "Barlow-Medium", "Barlow-Bold", "Barlow-SemiBold", "Barlow-SemiBold (Masked)", "Barlow-ExtraBold", "Barlow-BoldItalic", "Barlow-BoldItalic (Masked)", "Barlow-Black", "Barlow-Light", "Barlow-Regular", "Barlow-Regular (Masked)", "Barlow-Regular (Outline)", "Brook", "LiberationSans", "NotoSans", "VCR", "CONSOLA", "digital-7", "OCRAEXT", "DIN_Pro_Bold_700" };
	//const std::vector<const char*> MATERIALS = { "Barlow-Italic Outline", "Barlow-BoldItalic Outline", "Barlow-SemiBold Outline" };
	void Render();
}
