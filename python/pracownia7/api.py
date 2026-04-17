import asyncio
import aiohttp
import json
from rich import print_json
from private import API_KEY

def save_json(filename, data):
    with open(filename, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4, ensure_ascii=False)

# RapidAPI (z kluczem API)
async def get_streaming_data(session):
    url = "https://streaming-availability.p.rapidapi.com/shows/search/filters"

    params = {
        "country": "PL",
        "series_granularity": "show",
        "order_direction": "asc",
        "order_by": "original_title",
        "year_min": "2025",
        "output_language": "en",
        "catalogs": "netflix",
        "show_type": "movie"
    }

    headers = {
        "x-rapidapi-key": API_KEY,
        "x-rapidapi-host": "streaming-availability.p.rapidapi.com"
    }

    async with session.get(url, headers=headers, params=params) as resp:
        return await resp.json()


# dog ceo (bez autoryzacji)

async def get_random_dog(session):
    url = "https://dog.ceo/api/breed/husky/images/random"
    
    async with session.get(url) as resp:
        return await resp.json()

async def main():
    async with aiohttp.ClientSession() as session:
        streaming_task = asyncio.create_task(get_streaming_data(session))
        dog_task = asyncio.create_task(get_random_dog(session))

        streaming_data, dog_data = await asyncio.gather(streaming_task, dog_task)

        print_json(data=streaming_data)
        print_json(data=dog_data)

        save_json("streaming.json", streaming_data)
        save_json("dog.json", dog_data)

if __name__ == "__main__":
    asyncio.run(main())
