interface Ingredient {
  id: string;
  name: string;
}

interface Inventor {
  id: string;
  firstName: string;
  lastName: string;
}

interface Elixir {
  id: string;
  name: string;
  effect?: string;
  sideEffects?: string;
  characteristics?: string;
  time?: string;
  difficulty: string;
  ingredients: Ingredient[];
  inventors: Inventor[];
  manufacturer?: string;
}

interface Spell {
  id: string;
  name: string;
  incantation?: string;
  effect: string;
  canBeVerbal: boolean;
  type: string;
  light: string;
  creator?: string;
}

enum Endpoints {
  ELIXIRS = "elixirs",
  SPELLS = "spells"
}

let elixirs: Elixir[] = [];
let spells: Spell[] = [];
let validOption: string | undefined;

const gameContainer = document.getElementById("game")!;

async function fetchData<T>(endpoint: Endpoints): Promise<T[]> {
  const response = await fetch(
    `https://wizard-world-api.herokuapp.com/${endpoint}`
  );
  if (!response.ok) throw new Error(`Error fetching ${endpoint}`);
  return response.json();
}

async function fetchAllData() {
  [elixirs, spells] = await Promise.all([
    fetchData<Elixir>(Endpoints.ELIXIRS).then(data =>
      data.filter((elixir): elixir is Elixir & { effect: string } => !!elixir.effect)
    ),
    fetchData<Spell>(Endpoints.SPELLS).then(data =>
      data.filter((spell): spell is Spell & { incantation: string } => !!spell.incantation)
    )
  ]);
}

function getRandomElements<T>(array: T[], count: number): T[] {
  const indexes = new Set<number>();
  while (indexes.size < count) {
    indexes.add(Math.floor(Math.random() * array.length));
  }
  return Array.from(indexes).map(index => array[index]);
}

function generateOptions<T extends { name: string }>(randomElements: T[]) {
  const [rightOption, ...rest] = randomElements;
  const options = [rightOption, ...rest].sort(() => Math.random() > 0.5 ? 1 : -1);
  return { rightOption, options };
}

function elixirGame() {
  const { options, rightOption } = generateOptions(
    getRandomElements(elixirs, 3)
  );
  validOption = rightOption.name;
  renderOptions(
    `Which elixir effect is: "${rightOption.effect}"?`,
    options.map(elixir => elixir.name)
  );
}

function spellGame() {
  const { options, rightOption } = generateOptions(
    getRandomElements(spells, 3)
  );
  validOption = rightOption.name;
  renderOptions(
    `Which spell incantation is: "${rightOption.incantation}"?`,
    options.map(spell => spell.name)
  );
}

function renderOptions(question: string, answers: string[]) {
  const questionElement = document.getElementById("question")!;
  gameContainer.innerHTML = "";
  questionElement.textContent = question;

  answers.forEach(answer => {
    const option = document.createElement("button");
    option.textContent = answer;
    gameContainer.appendChild(option);
  });
}

gameContainer.addEventListener("click", (event: MouseEvent) => {
  const target = event.target as HTMLElement;
  if (target.tagName === "BUTTON") {
    const selectedOption = target.textContent;
    if (selectedOption === validOption) {
      round();
    } else {
      alert("Wrong answer!");
    }
  }
});

function round() {
  const randomGame = Math.random() > 0.5 ? elixirGame : spellGame;
  randomGame();
}

async function startGame() {
  await fetchAllData();
  round();
}

startGame();