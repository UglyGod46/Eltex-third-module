#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Contact.h"

Contact contacts[MAX_CONTACTS];
int contact_count = 0;

void load_contacts() {
    FILE *file = fopen(FILENAME, "rb");
    if (file) {
        contact_count = fread(contacts, sizeof(Contact), MAX_CONTACTS, file);
        fclose(file);
    }
}

void save_contacts() {
    FILE *file = fopen(FILENAME, "wb");
    if (file) {
        fwrite(contacts, sizeof(Contact), contact_count, file);
        fclose(file);
    }
}

void add_contact() {
    if (contact_count < MAX_CONTACTS) {
        printf("Введите фамилию: ");
        scanf("%s", contacts[contact_count].surname);
        printf("Введите имя: ");
        scanf("%s", contacts[contact_count].name);
        printf("Введите место работы: ");
        scanf("%s", contacts[contact_count].workplace);
        printf("Введите должность: ");
        scanf("%s", contacts[contact_count].position);
        printf("Введите номер телефона: ");
        scanf("%s", contacts[contact_count].phone);
        printf("Введите социальные сети: ");
        scanf("%s", contacts[contact_count].social);
        
        contact_count++;
        
        // Сохраняем контакты в файл сразу после добавления
        save_contacts();
        
        printf("Контакт добавлен и сохранён.\n");
    } else {
        printf("Достигнуто максимальное количество контактов.\n");
    }
}

void edit_contact() {
    char surname[50];
    printf("Введите фамилию контакта для редактирования: ");
    scanf("%s", surname);

    for (int i = 0; i < contact_count; i++) {
        if (strcmp(contacts[i].surname, surname) == 0) {
            printf("Редактирование контакта:\n");
            printf("Введите новое имя: ");
            scanf("%s", contacts[i].name);
            printf("Введите новое место работы: ");
            scanf("%s", contacts[i].workplace);
            printf("Введите новую должность: ");
            scanf("%s", contacts[i].position);
            printf("Введите новый номер телефона: ");
            scanf("%s", contacts[i].phone);
            printf("Введите новые социальные сети: ");
            scanf("%s", contacts[i].social);

            // Сохраняем контакты в файл после редактирования
            save_contacts();
            printf("Контакт изменён и сохранён.\n");
            return;
        }
    }
    printf("Контакт с фамилией %s не найден.\n", surname);
}

void delete_contact() {
    char surname[50];
    printf("Введите фамилию контакта для удаления: ");
    scanf("%s", surname);

    for (int i = 0; i < contact_count; i++) {
        if (strcmp(contacts[i].surname, surname) == 0) {
            // Сдвигаем все последующие контакты на одну позицию влево
            for (int j = i; j < contact_count - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            contact_count--;

            // Сохраняем контакты в файл после удаления
            save_contacts();
            printf("Контакт с фамилией %s удалён и изменения сохранены.\n", surname);
            return;
        }
    }
    printf("Контакт с фамилией %s не найден.\n", surname);
}

void display_contacts() {
    if (contact_count == 0) {
        printf("Список контактов пуст.\n");
        return;
    }
    for (int i = 0; i < contact_count; i++) {
        printf("Контакт %d:\n", i + 1);
        printf("Фамилия: %s\n", contacts[i].surname);
        printf("Имя: %s\n", contacts[i].name);
        printf("Место работы: %s\n", contacts[i].workplace);
        printf("Должность: %s\n", contacts[i].position);
        printf("Телефон: %s\n", contacts[i].phone);
        printf("Социальные сети: %s\n", contacts[i].social);
        printf("------------------------------\n");
    }
}
