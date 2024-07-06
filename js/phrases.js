

function howMuchILoveYou(nbPetals) {
  const Phrases = [
    "I love you",
    "a little",
    "a lot",
    "passionately",
    "madly",
    "not at all"
  ];
  const index = (nbPetals - 1) % 6;
  return Phrases[index];
}
