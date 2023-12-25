typedef struct {
    const char * class;
    const char * name;
    vec3_t origin;
} entity_t;

bool ent_matches(const entity_t* ent, const char* class, const char* name);
