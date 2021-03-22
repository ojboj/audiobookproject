import spacy
from spacy import displacy
from collections import Counter
import en_core_web_sm

nlp = en_core_web_sm.load()

def get_entities_from_text(text):
    # process the text
    processed_text = nlp(text)
    # get a list entity words and their types
    entities = [[word.start, word.text, word.label_] for word in processed_sentence.ents]
    return entities

def get_people_from_text(text):
    people = []

    # For each person in entities
    for entity_sentence_index, entity_text, entity_label in [entity for entity in entities if entity[2] == 'PERSON']:
        # get index of person with the same name in the 'people' list
        indexes_of_same_person = [person_index for person_index, person in enumerate(people) if person['name'] == entity_text]

        # TODO: check not greater than 1?

        # if no person by that name, form the person and add to list
        if len(indexes_of_same_person) == 0:
            person = {}
            person["name"] = entity_text
            person["indexes"] = [entity_sentence_index]
            people.append(person)
        # if there is, amend it to add this mention
        else:
            people[indexes_of_same_person[0]]["indexes"].append(entity_sentence_index)
