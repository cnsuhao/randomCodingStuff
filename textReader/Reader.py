#!/usr/bin/env python
# -*- coding: utf-8 -*-
# reader
# detects text in photo or video input and reads it out loud.
# useful for people who have hard time reading text.

#from espeak impoart espeak
#espeak.synth("Hello world!")

import talkey

# Colors
# https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
c_WHITE = "\033[0;37m"
c_RESET = "\033[0;49m"
c_GREEN = "\033[0;32m"
c_RED = "\033[0;31m"
c_BLUE = "\033[0;34m"
c_YELLOW = "\033[0;33m"

tts = talkey.Talkey(
    # These languages are given better scoring by the language detector
    # to minimise the chance of it detecting a short string completely incorrectly.
    # Order is not important here
    preferred_languages=['en', 'af', 'el', 'fr'],

    # The factor by which preferred_languages gets their score increased, defaults to 80.0
    preferred_factor=80.0,

    # The order of preference of using a TTS engine for a given language.
    # Note, that networked engines (Google, Mary) is disabled by default, and so is dummy
    # default: ['google', 'mary', 'espeak', 'festival', 'pico', 'flite', 'dummy']
    # This sets eSpeak as the preferred engine, the other engines may still be used
    #  if eSpeak doesn't support a requested language.
    engine_preference=['espeak'],
    # Here you segment the configuration by engine
    # Key is the engine SLUG, in this case ``espeak``
    espeak={
        # Specify the engine options:
        'options': {
            'enabled': True,
        },

        # Specify some default voice options
        'defaults': {
                'words_per_minute': 150,
                'variant': 'f4',
        },

        # Here you specify language-specific voice options
        # e.g. for english we prefer the mbrola en1 voice
        # 'english-mb-en1' gives error..
        'languages': {
            'en': {
                'voice': 'english-mb-en1',
                'words_per_minute': 130
            },
        }
    }
)

"""
# Here you segment the configuration by engine
    # Key is the engine SLUG, in this case ``espeak``
    espeak={
        # Specify the engine options:
        'options': {
            'enabled': True,
        },

        # Specify some default voice options
        'defaults': {
                'words_per_minute': 150,
                'variant': 'f4',
        },

        # Here you specify language-specific voice options
        # e.g. for english we prefer the mbrola en1 voice
        # 'english-mb-en1' gives error..
        'languages': {
            'en': {
                'voice': 'english-mb-en1',
                'words_per_minute': 130
            },
        }
    }
"""

global prefLang
prefLang = 'en'

def read(text):
	print(c_GREEN + str(text) + c_RESET)
	tts.say(text, prefLang)
	return
#tts.get_engine_for_lang(prefLang)
"""
finstring = "Hei Jani, mitä kuuluu?"
tts.say(finstring, 'fi')
"""
# whatif = "whatifiwritetexttogethercanyoureadit?"
#whatif2 = "what if i write text together can you read it?"
#read(whatif)
#read(whatif2)
#welcomeString = "Hello, this is a automatic text reader, that will read any text you give it."

forExample = "The quick brown fox jumps over the lazy dog"

#read(welcomeString)
print ""
read(forExample)
