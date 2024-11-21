#define MAX_CONTACTS 100
#define MAX_NAME_LENGTH 50
#define MAX_WORKPLACE_LENGTH 100
#define MAX_POSITION_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_SOCIAL_LENGTH 100
#define FILENAME "contacts.dat"

typedef struct {
    char surname[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    char workplace[MAX_WORKPLACE_LENGTH];
    char position[MAX_POSITION_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char social[MAX_SOCIAL_LENGTH];
} Contact;

extern Contact contacts[MAX_CONTACTS];
extern int contact_count;

void load_contacts();
void save_contacts();
void add_contact();
void edit_contact();
void delete_contact();
void display_contacts();


