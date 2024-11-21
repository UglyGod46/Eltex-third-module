#include <stdio.h>
#include "Contact.h"

int main() {
    load_contacts();
    int choice;

    while (1) {
        printf("1. Добавить контакт\n");
        printf("2. Изменить контакт\n");
        printf("3. Удалить контакт\n");
        printf("4. Показать все контакты\n");
        printf("5. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_contact();
                break;
            case 2:
                edit_contact();
                break;
            case 3:
                delete_contact();
                break;
            case 4:
                display_contacts();
                break;
            case 5:
                save_contacts();
                printf("Выход из программы.\n");
                return 0;
            default:
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }

    return 0;
}
